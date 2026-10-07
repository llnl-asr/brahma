Introduction
============

What is Brahma?
---------------

Brahma is a C++ I/O interception framework built on top of
`GOTCHA <https://github.com/LLNL/GOTCHA>`_, a function wrapping library from
LLNL. Instead of writing raw symbol-wrapping code, a tool developer subclasses
one of Brahma's interface classes (``brahma::POSIX``, ``brahma::STDIO``,
``brahma::MPI``, ``brahma::MPIIO``, or ``brahma::HDF5``), overrides the
virtual methods for the calls of interest, and Brahma installs the GOTCHA
bindings that route the application's calls through the subclass.

Features
--------

- Class-based interception: override only the calls you care about; all
  other calls fall through to the original implementation.
- Interfaces for POSIX, STDIO, MPI, MPI-IO, and HDF5. MPI/MPI-IO and HDF5
  support are optional at build time.
- Tool stacking through GOTCHA: each tool binds with a unique name and a
  priority, so multiple interception tools can coexist.
- Singleton-based instance management (``brahma::Singleton<T>``) so
  interceptors can be installed from library constructors or initialization
  routines.
- Optional logging through `cpp-logger
  <https://github.com/llnl-asr/cpp-logger>`_ with compile-time log levels.

Building
--------

Brahma uses CMake. Dependencies (GOTCHA and cpp-logger) can be fetched and
built automatically:

.. code-block:: bash

   cd <CLONED_REPO>
   cmake -DBRAHMA_BUILD_DEPENDENCIES=ON -S $PWD -B $PWD/build
   cmake --build $PWD/build -j

If the dependencies are already installed, configure with
``-DBRAHMA_BUILD_DEPENDENCIES=OFF``.

Key CMake options:

===========================  ==================================================
Option                       Description
===========================  ==================================================
BRAHMA_BUILD_DEPENDENCIES    Fetch and build GOTCHA / cpp-logger
BRAHMA_BUILD_WITH_HDF5       Enable HDF5 interception
BRAHMA_BUILD_WITH_MPI        Enable MPI and MPI-IO interception
BRAHMA_ENABLE_TESTING        Enable tests
BRAHMA_GENERATE_INTERFACES   Regenerate interface sources
BRAHMA_LOGGER                Logger backend (``CPP_LOGGER``)
BRAHMA_LOGGER_LEVEL          Compile-time log level (TRACE .. ERROR)
BRAHMA_CPP_LOGGER_GIT_URL    Git URL used to fetch cpp-logger (defaults to
                             GitHub; can point at a mirror)
===========================  ==================================================

To use Brahma from another CMake project:

.. code-block:: cmake

   find_package(brahma 2.0.0 REQUIRED)

Quick start
-----------

Using Brahma has three steps: derive an interface class, bind it, and unbind
it when done.

.. code-block:: c++

   #include <brahma/brahma.h>

   class POSIXTool : public brahma::POSIX {
    public:
     int open(const char *pathname, int flags, ...) override;
     ssize_t read(int fd, void *buf, size_t count) override;
     // ... override only the calls you need
   };

   // In your library constructor / init routine:
   auto posix = POSIXTool::get_instance();
   posix->bind<POSIXTool>("my_tool", /*priority=*/1);

   // In your library destructor / finalize routine:
   posix->unbind();

The tool name must be unique per tool (GOTCHA uses it to stack tools) and the
priority controls ordering when multiple tools intercept the same calls.
