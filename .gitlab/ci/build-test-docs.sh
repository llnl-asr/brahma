#!/bin/bash
# Runs ON the allocated compute node (invoked via
#   flux proxy <jobid> flux run -N 1 bash .gitlab/ci/build-test-docs.sh)
# and executes the CI inside podman containers using the same images as the
# GitHub Actions CI (hdevarajan92/brahma-ci for build/test, python:3.11 for
# docs) so GitLab CI mimics GitHub CI as closely as possible.
set -ex

# Rootless podman needs node-local storage (overlayfs does not work on NFS
# homes/workspaces). Keep image store + runroot in /var/tmp on the node.
PODMAN_STORE=/var/tmp/$USER/podman-root
PODMAN_RUNROOT=/var/tmp/$USER/podman-run
mkdir -p "$PODMAN_STORE" "$PODMAN_RUNROOT"
PODMAN="podman --root $PODMAN_STORE --runroot $PODMAN_RUNROOT"

# --user 0:0 is REQUIRED: images with a non-root USER (e.g. brahma-ci) map to a
# subuid under rootless podman and cannot read the bind-mounted checkout
# ("Permission denied" sourcing load_env.sh). Container root maps to the host
# user, which owns the files.
# --user 0:0: container root maps to the host user under rootless podman, so
# the bind-mounted checkout stays readable even for images with a non-root USER.
# The cpp-logger CMake dependency is fetched from czgitlab over ssh, so the
# container also needs the runner account's keys mounted read-only.
PODMAN_RUN_OPTS=(
  --rm --user 0:0
  -v "$PWD:/ws" -w /ws
  -v "$HOME/.ssh:/root/.ssh:ro"
  -e GIT_SSH_COMMAND="ssh -o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null"
)

# The GitHub CI ran a 4-way spack matrix inside this image:
#   hdf5@1.8.23  / mpich@3.4.3
#   hdf5@1.10.11 / openmpi@4.1.6
#   hdf5@1.12.3  / mpich@4.2.3
#   hdf5@1.14.5  / openmpi@5.0.6
# We keep ONE combo here so the pipeline fits in the shared allocation's
# walltime; the other three can be enabled by looping over them below (or by
# adding a GitLab `parallel: matrix:` with its own allocation).
HDF5=${HDF5:-hdf5@1.14.5^openmpi@5.0.6}
MPI=${MPI:-openmpi@5.0.6}

$PODMAN run "${PODMAN_RUN_OPTS[@]}" \
  -e HDF5="$HDF5" -e MPI="$MPI" \
  docker.io/hdevarajan92/brahma-ci:latest bash -ec '
  # Spack toolchain from the image, exactly as the GitHub CI did it.
  source .github/workflows/scripts/load_env.sh "$HDF5" "$MPI"
  mkdir -p build
  cmake -Bbuild \
    -DCMAKE_BUILD_TYPE=Debug \
    -DBRAHMA_ENABLE_TESTING=ON \
    -DBRAHMA_GENERATE_INTERFACES=OFF \
    -DBRAHMA_BUILD_WITH_HDF5=ON \
    -DBRAHMA_BUILD_WITH_MPI=ON \
    -DHDF5_ROOT="$HDF5_DIR" \
    -DMPI_ROOT="$MPI_DIR" \
    -DBRAHMA_BUILD_DEPENDENCIES=ON
  make -C build -j "$(nproc)"
  ctest --test-dir build -VV
'

# Docs in the same image the pages job used.
$PODMAN run "${PODMAN_RUN_OPTS[@]}" docker.io/library/python:3.11 bash -ec '
  pip install -r docs/requirements.txt
  sphinx-build -b html docs public
'
