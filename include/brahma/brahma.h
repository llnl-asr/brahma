//
// Created by hariharan on 8/8/22.
//

#ifndef BRAHMA_BRAHMA_H
#define BRAHMA_BRAHMA_H

#include <brahma/brahma_config.hpp>
/* Internal Headers */
#ifdef BRAHMA_ENABLE_MPI
#include <brahma/interface/mpi.h>
#include <brahma/interface/mpiio.h>
#include <brahma/interface/mpi_compat.h>
#include <brahma/interface/mpiio_compat.h>
#endif
#ifdef BRAHMA_ENABLE_HDF5
#include <brahma/interface/hdf5.h>
#endif
#include <brahma/interface/posix.h>
#include <brahma/interface/stdio.h>
/* External Headers */
#include <cassert>

#endif  // BRAHMA_BRAHMA_H
