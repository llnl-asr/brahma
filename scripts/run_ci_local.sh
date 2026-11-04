#!/usr/bin/env bash
#
# Reproduce the GitHub Actions CI build+test locally, using the exact same
# container image (hdevarajan92/brahma-ci:mpi-wide) as .github/workflows/ci.yml,
# via either podman or docker -- whichever is available.
#
# Why this exists: CI failures are sometimes specific to the container's
# glibc/toolchain version (e.g. a function silently unbound by gotcha, or a
# glibc symbol redirect that only exists on newer distros) and don't
# reproduce on a developer's own machine. This script runs the same image
# CI uses, so those failures can be reproduced and debugged locally.
#
# Engine selection: podman is preferred when both are available or when
# only podman is installed (e.g. on LLNL's Tuolumne and similar systems,
# where podman is provided instead of docker). Override with --engine.
#
# Why copy-in instead of a bind mount: the container runs as user "spack"
# (uid 1001). A bind-mounted source directory keeps the *host* file
# permissions, which are commonly too restrictive for uid 1001 to read
# (this is what happens by default on many clusters, including Tuolumne).
# Rather than requiring callers to loosen permissions or figure out
# --userns=keep-id / -v :Z relabeling for their specific engine and
# storage driver, this script copies the source into the container's own
# filesystem (podman/docker cp) and builds entirely there. This works
# identically for both engines and regardless of host directory
# permissions. The build directory stays inside the container; use --keep
# plus '<engine> cp' to pull specific artifacts out afterward if needed.
#
# Usage:
#   scripts/run_ci_local.sh [options]
#
# Options:
#   --engine <podman|docker|auto>   Container engine to use (default: auto)
#   --hdf5 <spec>                   HDF5 spack spec, e.g. hdf5@1.14.5 (default: first matrix entry)
#   --mpi <spec>                    MPI spack spec, e.g. openmpi@5.0.6 (default: first matrix entry)
#   --all                           Run the full CI matrix (all 4 combinations) instead of one
#   --keep                          Don't remove the container when done (for interactive debugging)
#   --shell                         Drop into an interactive shell in the container instead of running CI
#   -h, --help                      Show this help
#
# Examples:
#   scripts/run_ci_local.sh
#   scripts/run_ci_local.sh --hdf5 hdf5@1.10.11 --mpi openmpi@4.1.6
#   scripts/run_ci_local.sh --all
#   scripts/run_ci_local.sh --keep --shell

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"
IMAGE="hdevarajan92/brahma-ci:mpi-wide"
CONTAINER_NAME="brahma-ci-local-$$"
CONTAINER_SRC_DIR="/home/spack/brahma"

# Matrix combinations, matching .github/workflows/ci.yml exactly.
MATRIX_HDF5=("hdf5@1.8.23^mpich@3.4.3" "hdf5@1.10.11^openmpi@4.1.6" "hdf5@1.10.11^openmpi@4.1.8" "hdf5@1.12.3^mpich@4.2.3" "hdf5@1.14.5^mpich@4.3.0" "hdf5@1.14.5^openmpi@5.0.3" "hdf5@1.14.5^openmpi@5.0.6")
MATRIX_MPI=("mpich@3.4.3" "openmpi@4.1.6" "openmpi@4.1.8" "mpich@4.2.3" "mpich@4.3.0" "openmpi@5.0.3" "openmpi@5.0.6")

ENGINE="auto"
HDF5_SPEC=""
MPI_SPEC=""
RUN_ALL=0
KEEP_CONTAINER=0
INTERACTIVE_SHELL=0

usage() {
  sed -n '2,40p' "${BASH_SOURCE[0]}" | sed 's/^# \{0,1\}//'
}

while [ $# -gt 0 ]; do
  case "$1" in
    --engine) ENGINE="$2"; shift 2 ;;
    --hdf5) HDF5_SPEC="$2"; shift 2 ;;
    --mpi) MPI_SPEC="$2"; shift 2 ;;
    --all) RUN_ALL=1; shift ;;
    --keep) KEEP_CONTAINER=1; shift ;;
    --shell) INTERACTIVE_SHELL=1; shift ;;
    -h|--help) usage; exit 0 ;;
    *) echo "Unknown option: $1" >&2; usage; exit 1 ;;
  esac
done

# ---------------------------------------------------------------------------
# Engine detection
# ---------------------------------------------------------------------------
detect_engine() {
  if [ "$ENGINE" != "auto" ]; then
    echo "$ENGINE"
    return
  fi
  if command -v podman >/dev/null 2>&1; then
    echo "podman"
  elif command -v docker >/dev/null 2>&1; then
    echo "docker"
  else
    echo "ERROR: neither podman nor docker found on PATH" >&2
    exit 1
  fi
}

ENGINE="$(detect_engine)"
if ! command -v "$ENGINE" >/dev/null 2>&1; then
  echo "ERROR: requested engine '$ENGINE' not found on PATH" >&2
  exit 1
fi
echo "[run_ci_local] Using container engine: $ENGINE"

# ---------------------------------------------------------------------------
# Container lifecycle
# ---------------------------------------------------------------------------
cleanup() {
  if [ "$KEEP_CONTAINER" -eq 0 ]; then
    "$ENGINE" rm -f "$CONTAINER_NAME" >/dev/null 2>&1 || true
  else
    echo "[run_ci_local] --keep set: leaving container running as '$CONTAINER_NAME'"
    echo "[run_ci_local]   Attach with: $ENGINE exec -it $CONTAINER_NAME bash"
    echo "[run_ci_local]   Remove with: $ENGINE rm -f $CONTAINER_NAME"
  fi
}
STAGING_DIR="$(mktemp -d)"
cleanup_all() {
  rm -rf "$STAGING_DIR"
  cleanup
}
trap cleanup_all EXIT

echo "[run_ci_local] Pulling ${IMAGE} ..."
"$ENGINE" pull "$IMAGE"

echo "[run_ci_local] Starting container ${CONTAINER_NAME} ..."
"$ENGINE" run -d --name "$CONTAINER_NAME" "$IMAGE" sleep infinity >/dev/null

echo "[run_ci_local] Copying source tree into container (excluding build dirs) ..."
# Copy into a staging dir on the host first so we can exclude build*/ and
# .git/ without needing tar/rsync support inside the (possibly minimal)
# container image.
tar -C "$REPO_ROOT" \
  --exclude='./build' --exclude='./build-*' --exclude='./.git' \
  -cf - . | tar -C "$STAGING_DIR" -xf -

"$ENGINE" exec "$CONTAINER_NAME" mkdir -p "$CONTAINER_SRC_DIR"
"$ENGINE" cp "$STAGING_DIR/." "$CONTAINER_NAME:$CONTAINER_SRC_DIR/"
"$ENGINE" exec --user root "$CONTAINER_NAME" chown -R spack:spack "$CONTAINER_SRC_DIR"

if [ "$INTERACTIVE_SHELL" -eq 1 ]; then
  echo "[run_ci_local] Dropping into interactive shell (source is at $CONTAINER_SRC_DIR) ..."
  echo "[run_ci_local]   Load an environment with:"
  echo "[run_ci_local]     source .github/workflows/scripts/load_env.sh <hdf5-spec> <mpi-spec>"
  KEEP_CONTAINER=1
  "$ENGINE" exec -it -w "$CONTAINER_SRC_DIR" "$CONTAINER_NAME" bash
  exit 0
fi

run_one_combination() {
  local hdf5="$1" mpi="$2"
  local build_dir="build-${hdf5//[@\/]/_}-${mpi//[@\/]/_}"

  echo ""
  echo "==================================================================="
  echo "[run_ci_local] Building + testing: ${hdf5}  /  ${mpi}"
  echo "==================================================================="

  "$ENGINE" exec "$CONTAINER_NAME" bash -lc "
    set -euo pipefail
    source /home/spack/spack/share/spack/setup-env.sh
    spack load '${hdf5}'
    spack load '${mpi}'
    spack load cmake
    export HDF5_DIR=\$(spack location -i '${hdf5}')
    export MPI_DIR=\$(spack location -i '${mpi}')
    export CC=\$(which mpicc)
    export CXX=\$(which mpic++)
    cd '${CONTAINER_SRC_DIR}'
    rm -rf '${build_dir}'
    mkdir -p '${build_dir}'
    cmake -B'${build_dir}' \
      -DCMAKE_BUILD_TYPE=Debug \
      -DBRAHMA_ENABLE_TESTING=ON \
      -DBRAHMA_GENERATE_INTERFACES=OFF \
      -DBRAHMA_BUILD_WITH_HDF5=ON \
      -DBRAHMA_BUILD_WITH_MPI=ON \
      -DHDF5_ROOT=\"\$HDF5_DIR\" \
      -DMPI_ROOT=\"\$MPI_DIR\" \
      -DBRAHMA_BUILD_DEPENDENCIES=ON
    make -C '${build_dir}' -j\"\$(nproc)\"
    ctest --test-dir '${build_dir}' -VV
  "
}

if [ "$RUN_ALL" -eq 1 ]; then
  status=0
  for i in "${!MATRIX_HDF5[@]}"; do
    if ! run_one_combination "${MATRIX_HDF5[$i]}" "${MATRIX_MPI[$i]}"; then
      status=1
      echo "[run_ci_local] FAILED: ${MATRIX_HDF5[$i]} / ${MATRIX_MPI[$i]}"
    fi
  done
  exit "$status"
else
  hdf5="${HDF5_SPEC:-${MATRIX_HDF5[0]}}"
  mpi="${MPI_SPEC:-${MATRIX_MPI[0]}}"
  run_one_combination "$hdf5" "$mpi"
fi
