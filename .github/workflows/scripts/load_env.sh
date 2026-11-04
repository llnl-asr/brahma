#!/bin/bash
# Load spack packages and compile with their MPI wrappers.
# Usage: source load_env.sh <spec> [<spec>...]
#   CI loads one: an MPI (openmpi@4.1.8) or an HDF5 named by its MPI (hdf5@1.10.11^openmpi@4.1.6).
#   Older callers pass an HDF5 and an MPI, and read HDF5_DIR and MPI_DIR.
# PACKAGE_DIR is the first package's prefix. spack load also puts an HDF5's MPI on PATH,
# which a parallel HDF5 needs for mpi.h.
if [ $# -lt 1 ]; then
    echo "Usage: source load_env.sh <spack spec> [<spack spec>...]" >&2
    return 1 2>/dev/null || exit 1
fi
source /home/spack/spack/share/spack/setup-env.sh
for spec in "$@"; do
    spack load "$spec"
done
spack load cmake
export PACKAGE_DIR=$(spack location -i "$1")
case "$1" in hdf5*) export HDF5_DIR=$PACKAGE_DIR ;; esac
[ -n "$2" ] && export MPI_DIR=$(spack location -i "$2")
export CC=$(which mpicc)
export CXX=$(which mpic++)
echo "loaded $*; PACKAGE_DIR=$PACKAGE_DIR CC=$CC CXX=$CXX"
spack find --loaded
