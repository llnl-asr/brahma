# Brahma v1.0.2

A C++ style interception library for application calls.
This library uses [GOTCHA](https://github.com/LLNL/GOTCHA) to intercept POSIX, STDIO, MPI-IO, and HDF5 calls.
The interception of MPI-IO and HDF5 calls are optional and the library can be compiled without MPI-IO or HDF5.

## Dependencies

1. GOTCHA v1.0.8
2. CPP Logger v0.0.7

## Build Brahma with cmake

Users can install the dependency using cmake.

```bash
cd <CLONED_REPO>
mkdir build
cmake -DBRAHMA_BUILD_DEPENDENCIES=ON -S$PWD -B$PWD/build -G "Unix Makefiles"
cmake --build $PWD/build --target all -j 50
```

User can install the library using cmake.

```bash
cd <CLONED_REPO>
mkdir build
cmake -DBRAHMA_BUILD_DEPENDENCIES=OFF -S$PWD -B$PWD/build -G "Unix Makefiles"
cmake --build $PWD/build --target all -j 50
```

### Options available in Brahma

| Option                      | Description                                                                                 |
| --------------------------- | ------------------------------------------------------------------------------------------- |
| BRAHMA_BUILD_DEPENDENCIES   | Build Brahma dependencies                                                                   |
| BRAHMA_BUILD_WITH_HDF5      | Enable HDF5 interceptions                                                         |
| BRAHMA_BUILD_WITH_MPI       | Enable MPI and MPI-IO interceptions                                                         |
| BRAHMA_ENABLE_TESTING       | Enable testing                                                                              |
| BRAHMA_GENERATE_INTERFACES  | Generate interfaces                                                                         |
| BRAHMA_INSTALL_DEPENDENCIES | Install Brahma dependencies into `CMAKE_INSTALL_PREFIX`. If off its kept in build directory |
| BRAHMA_LOGGER               | Enable Logging for CPP Logger (`CPP_LOGGER`)                                                |
| BRAHMA_LOGGER_LEVEL         | Set compile time logging level (`TRACE`, `DEBUG`, `INFO`, `WARN`, and `ERROR`)              |
| BRAHMA_MPI_IMPL             | Override the detected MPI implementation (`OPENMPI`, `MPICH`, `MVAPICH`, `CRAYMPICH`, `INTELMPI`, `UNKNOWN`) |
| BRAHMA_MPI_VERSION          | Override the detected MPI library version, as `major*100000 + minor*100 + patch` (e.g. `500003` for 5.0.3). With `BRAHMA_MPI_IMPL` set too, detection is skipped |

## Including brahma in your CMake Project.

```cmake
find_package(brahma 2.0.0 REQUIRED)
```

Exported variables for this package are `brahma_FOUND`, `BRAHMA_INCLUDE_DIRS`, and `BRAHMA_LIBRARIES`.

## Using Brahma library to intercept calls.

There are three main steps to use Brahma:

1. Enable bindings
2. Override I/O Classes
3. Disable bindings

### Binding functions

The Binding has to be done before any interception can happen.
Some recommended places are library constructor or initialization routines.
The libraries which use Brahma can initiate binding of system calls on each interface using the `bind` function.
It takes two arguments. `UNIQUE_TOOL_NAME` should be a tool name that is used for stacking different interception tools used by gotcha.
`PRIORITY` is the prioritization of the tool.

```c++
auto posix = brahma::POSIXTest::get_instance();
posix->bind<brahma::POSIXTest>(UNIQUE_TOOL_NAME, PRIORITY);
```

### Disable bindings

This routine should be called to stop the interception.
Some recommended choices for this are library destructor or finalization routine.
The free bindings call release the interception bindings from brahma.

```c++
auto posix = brahma::POSIXTest::get_instance();
posix->unbind();
```

### Override I/O Classes

By default brahma will intercept all I/O calls. 
Tools can selectively choose which I/O calls they want to access by overriding the functions within the brahma classes.
The three classes `brahma::POSIX`, `brahma::STDIO`, and `brahma::MPIIO`.

```c++
namespace brahma {
    class POSIXInterceptor : public POSIX {
        int open(const char *pathname, int flags, ...) override;

        int creat64(const char *path, mode_t mode) override;

        int open64(const char *path, int flags, ...) override;

        // ...
    };
} // namespace brahma
```

```c++
namespace brahma {
    class STDIOInterceptor : public STDIO {
        FILE *fopen(const char *path, const char *mode) override;

        FILE *fopen64(const char *path, const char *mode) override;

        int fclose(FILE *fp) override;

        // ...
    };
} // namespace brahma
```

```c++
namespace brahma {
    class MPIIOInterceptor : public MPIIO {
        int MPI_File_close(MPI_File *fh) override;
        int MPI_File_open(MPI_Comm comm, const char *filename, int amode,
                            MPI_Info info, MPI_File *fh) override;
        int MPI_File_read_at_all(MPI_File fh, MPI_Offset offset, void *buf,
                                    int count, MPI_Datatype datatype,
                                    MPI_Status *status) override;
        int MPI_File_write_at_all(MPI_File fh, MPI_Offset offset, const void *buf,
                                    int count, MPI_Datatype datatype,
                                    MPI_Status *status) override;
        // ...
    };
} // namespace brahma
```

## Auto Interface Code Generation

This Python script generates interface and implementation files for specific libraries (currently MPI-IO and HDF5) used with the Brahma framework.

Enable auto-generation during the build process:

* Set `-DBRAHMA_GENERATE_INTERFACES=ON` in your CMake configuration.
* Ensure `LLVM` and `Python` are installed (required for build-time auto-generation).

### Manual Usage

You can also run the script manually with the following command:

```bash
python3 generate_interfaces.py --libclang-path <path_to_libclang> --hdf5-header-path <path_to_hdf5_header> --hdf5-version <version_of_hdf5> --mpi-header-path <path_to_mpi_header> --mpi-version <version_of_mpi> [--with-tests] [--verbose]
```

* `--libclang-path`: Path to the libclang library
* `--hdf5-header-path`: Path to the HDF5 library header file
* `--hdf5-version`: Version of the HDF5 library
* `--mpi-header-path`: Path to the MPI header file (generates both MPI and MPIIO interfaces)
* `--mpi-version`: Version of the MPI library
* `--mpi-implementation`: MPI implementation of each header (`openmpi`, `mpich`, `mvapich`, `craympich`)
* `--mpi-layered`: Generate MPI as a common interface plus compat interfaces (see below)
* `--with-tests`: Generate test cases during generation
* `--verbose`: Print verbose output (optional)

The script generates two files for each interface:

1. An interface header file (`<interface_name>.h`) in the `../include/brahma/interface/` directory
2. An implementation file (`<interface_name>.cpp`) in the `../src/brahma/interface/` directory

With `--with-tests` it also writes `test_<interface_name>.cpp` in `../test/`. With `--mpi-layered` the MPI
interfaces are split into a common interface and compat interfaces, each with its own header, implementation and
test (see below). Give `--mpi-header-path`, `--mpi-version` and `--mpi-implementation` once per MPI, in the same
order.

### MPI: common interface plus compat interfaces

With `--mpi-layered` (the default in `tools/discover_and_generate.sh`) MPI is split in two, so that any MPI
implementation and version works, including ones the generator has never seen.

* The **common interface** (`mpi.h`/`mpi.cpp`, `mpiio.h`/`mpiio.cpp`) holds every function whose signature is
  identical in all the MPI headers given to the generator. It has no implementation or version guards and is
  compiled for every MPI.
* A **compat interface** holds the functions its MPI has beyond that set, for example the MPI-4 large-count
  `_c` functions. There is one per implementation and major.minor, named
  `mpi_<impl>_<major>_<minor>_compat` (and `mpiio_...`), for example `mpi_openmpi_5_0_compat`. It is
  compiled only when the build's `BRAHMA_MPI_IMPL` and `BRAHMA_MPI_VERSION` fall in that family, and to nothing
  otherwise. It covers every patch release of the family.

An MPI with no compat interface still gets the common functions. A function that is in only some patch releases
of a family is left out of that family's compat interface, and the generator prints a warning listing them.
`mpi_compat.h` and `mpiio_compat.h` include every compat interface, and provide `brahma::mpi_compat_families()`
and `brahma::mpi_compat_enabled()` (and the `mpiio_` equivalents), which list the compat interfaces that exist and the
ones compiled in for the current build.

Signatures are compared by type, so `const char *x` and `const char x[]` count as the same.

### Adding an MPI release or implementation

1. Install the MPI (for example `spack install openmpi@5.0.7`) and note the directory that holds its `mpi.h`.
2. Regenerate with **every** MPI the interfaces should cover, the new one included. The common interface is the
   intersection of all the headers given, so leaving one out changes it for everyone. The current list is in the
   header comment of `include/brahma/interface/mpi.h`, under "Generated from these MPI headers". For example:

   ```bash
   cd tools
   python3 generate_interfaces.py --libclang-path <libclang.so> --with-tests --mpi-layered \
     --mpi-header-path <openmpi-4.1.6>/include <openmpi-5.0.6>/include <mpich-4.2.3>/include ... \
     --mpi-version 4.1.6 5.0.6 4.2.3 ... \
     --mpi-implementation openmpi openmpi mpich ...
   ```

   `tools/discover_and_generate.sh` finds the MPI and HDF5 installations loaded through Spack or modules and runs
   the same command.
3. Read the generator's `[mpi] layered:` line (functions per family) and any warning about functions left out of a
   family. A new major.minor gets a new compat interface; a new patch release of a known family usually only
   changes counts.
4. Build and test against the new MPI (next section), then add it to the CI matrix: an `hdf5@x+mpi ^<mpi>@<version>`
   line in `infrastructure/docker/Dockerfile.ci`, a matching entry in `.github/workflows/ci.yml`, and the same pair
   in `scripts/run_ci_local.sh`. The CI image must be rebuilt before the new entry can pass.

### Testing the MPI interfaces

Build with tests against the MPI to check, and run `ctest`:

```bash
cmake -S . -B build -DBRAHMA_ENABLE_TESTING=ON -DBRAHMA_BUILD_WITH_MPI=ON -DBRAHMA_BUILD_DEPENDENCIES=ON \
      -DMPI_ROOT=<mpi prefix> -DCMAKE_C_COMPILER=<mpi prefix>/bin/mpicc -DCMAKE_CXX_COMPILER=<mpi prefix>/bin/mpicxx
cmake --build build -j
ctest --test-dir build
```

Configure prints the detected implementation and version and the compat interface the build selects, for example
`MPI compat interface expected for this build: 'openmpi_5_0'`. The tests then check:

* `test_mpi` and `test_mpiio` bind and call every common function.
* `test_mpi_compat_select` checks that exactly the selected compat interface is compiled in, or none when brahma
  has no compat interface for this MPI.
* `test_mpi_<impl>_<major>_<minor>_compat` (and `test_mpiio_...`) calls every function of one compat interface. The
  one for this build must pass; it fails if it does not apply. Every other compat test exits 77 and `ctest` reports
  it as **skipped**, not passed.

To check the path an unseen MPI takes, force a version or implementation brahma has no compat interface for. The
common tests and `test_mpi_compat_select` must pass and every compat test must be skipped:

```bash
cmake ... -DBRAHMA_MPI_IMPL=OPENMPI -DBRAHMA_MPI_VERSION=509900   # OpenMPI "5.99"
cmake ... -DBRAHMA_MPI_IMPL=UNKNOWN -DBRAHMA_MPI_VERSION=400203
```

To run the whole CI matrix locally in the CI image, use `scripts/run_ci_local.sh --all`.

## Supported MPI

Every MPI implementation and version builds and traces the **common** MPI and MPI-IO functions: the ones whose
signatures are identical in all the MPIs below (see [MPI: common interface plus compat
interfaces](#mpi-common-interface-plus-compat-interfaces)). That includes versions not listed here. The
implementation-specific functions beyond them, such as the MPI-4 large-count `_c` calls, are traced only for the
families that have a **compat interface**.

| Implementation | Generated from        | Compat interface for          | CI matrix |
|----------------|-----------------------|-------------------------------|-----------|
| OpenMPI        | 4.1.6, 5.0.3, 5.0.6   | 4.1.x, 5.0.x                  | 4.1.6, 4.1.8, 5.0.3, 5.0.6 |
| MPICH          | 3.4.3, 4.2.3          | 3.4.x, 4.2.x (MPI-IO: 4.2.x)  | 3.4.3, 4.2.3, and 4.3.0 (no compat interface: common functions only) |
| MVAPICH2       | 2.3.7                 | 2.3.x                         | not in CI: the spack build of 2.3.7-1 fails |
| Cray MPICH     | 8.1.8, 8.1.33, 9.0.1, 9.1.0 | 8.1.x, 9.0.x, 9.1.x (MPI-IO: 9.0.x, 9.1.x) | not in CI: proprietary |

A compat interface covers every patch release of its family. Which functions are common or compat-only comes
from the exact headers the generator was given. Each generated file records them in its header comment, under
"Generated from these MPI headers (implementation version)", and the generator prints the per-family function
counts on its `layered:` line. A patch release that was not read is covered only if it has the same functions as
the ones that were. To support another MPI release or implementation, follow
[Adding an MPI release or implementation](#adding-an-mpi-release-or-implementation).

The HDF5 interface is still version-gated: HDF5 1.8.23, 1.10.5, 1.12.3 and 1.14.5 and later patch releases in the
same series.

## License

Brahma is distributed under the terms of the MIT license.
All new contributions must be made under this license.

See [LICENSE](LICENSE) and [NOTICE](NOTICE) for details.

SPDX-License-Identifier: MIT

LLNL-CODE-2024514 — Applied Storage Research (ASR)
