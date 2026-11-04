API Reference
=============

All public headers live under ``include/brahma``. The umbrella header
``brahma/brahma.h`` includes the enabled interfaces (POSIX and STDIO always;
MPI/MPI-IO when built with ``BRAHMA_BUILD_WITH_MPI``; HDF5 when built with
``BRAHMA_BUILD_WITH_HDF5``).

Core headers
------------

``brahma/brahma.h``
   Umbrella header. Include this to get every interface enabled in the build.

``brahma/interceptor.h``
   GOTCHA glue used by the generated interfaces: macros that declare typed
   function pointers, wrapper functions that forward an intercepted call to
   the active interface instance (falling back to the real function when no
   instance is set), and helpers that register bindings/unbindings with
   GOTCHA. Tool authors normally do not use these macros directly.

``brahma/singleton.h``
   ``brahma::Singleton<T>`` — a shared-pointer based singleton template used
   to manage interface instances.

   .. code-block:: c++

      template <typename T>
      class Singleton {
       public:
        template <typename... Args>
        static std::shared_ptr<T> get_instance(Args... args);
      };

Interface base class
--------------------

``brahma/interface/interface.h`` defines ``brahma::Interface``, the base of
all interception classes. It holds the GOTCHA binding tables and the tool
identity:

.. code-block:: c++

   namespace brahma {
   class Interface {
    protected:
     std::shared_ptr<InterfaceUtility> utility;
     std::vector<gotcha_binding_t> bindings;
     std::vector<gotcha_binding_t> unbindings;

    public:
     Interface();
     char tool_name[64];
     size_t num_bindings;
     int bind_priority;
   };
   }

Every concrete interface follows the same pattern:

.. code-block:: c++

   class POSIX : public Interface {
    public:
     static std::shared_ptr<POSIX> get_instance();
     static int set_instance(std::shared_ptr<POSIX> instance);

     template <typename C>
     size_t bind(const char *name, uint16_t priority);  // install GOTCHA hooks
     size_t unbind();                                   // remove GOTCHA hooks

     // virtual methods, one per interceptable call ...
   };

``bind<C>()`` only installs hooks for methods that class ``C`` actually
overrides, so unmodified calls keep their native fast path.

Interface classes
-----------------

``brahma::POSIX`` (``brahma/interface/posix.h``)
   Intercepts POSIX file and filesystem calls. Key virtual methods include
   ``open``/``open64``/``openat``, ``creat64``, ``close``, ``read``/``write``,
   ``pread``/``pwrite`` (and 64-bit variants), ``lseek``/``lseek64``,
   ``fsync``, ``fdatasync``, stat-family calls (``__xstat``, ``__fxstat``,
   ``__lxstat`` and 64-bit variants), and directory/metadata calls such as
   ``mkdir``, ``rmdir``, ``chdir``, and ``getcwd``.

``brahma::STDIO`` (``brahma/interface/stdio.h``)
   Intercepts C stdio stream calls: ``fopen``/``fopen64``, ``fclose``,
   ``fread``, ``fwrite``, ``fseek``, ``ftell``, and related stream
   operations.

``brahma::MPI`` (``brahma/interface/mpi.h``)
   Intercepts core MPI lifecycle calls (e.g. ``MPI_Init``, ``MPI_Finalize``)
   so tools can hook startup and shutdown of MPI applications. Available when
   built with ``BRAHMA_BUILD_WITH_MPI``.

``brahma::MPIIO`` (``brahma/interface/mpiio.h``)
   Intercepts MPI-IO calls: ``MPI_File_open``, ``MPI_File_close``,
   ``MPI_File_read*``/``MPI_File_write*`` variants (at/all/shared), seeks,
   and sync. Available when built with ``BRAHMA_BUILD_WITH_MPI``.

   ``brahma::MPI`` and ``brahma::MPIIO`` hold the functions that every
   supported MPI declares with the same signature, so they are available for
   any MPI implementation and version.

``brahma::MPI_<IMPL>_<MAJOR>_<MINOR>_COMPAT`` and ``brahma::MPIIO_..._COMPAT`` (``brahma/interface/mpi_<impl>_<major>_<minor>_compat.h``)
   The functions one MPI family has beyond the common set, for example the
   MPI-4 large-count ``_c`` calls in ``brahma::MPI_OPENMPI_5_0_COMPAT``. A
   compat class exists only when the build's MPI implementation, major and
   minor version match, which is signalled by
   ``BRAHMA_MPI_<IMPL>_<MAJOR>_<MINOR>_COMPAT``. ``brahma/interface/mpi_compat.h``
   includes all of them; ``brahma::mpi_compat_enabled()`` lists the ones
   compiled in. A tool binds each compat class it uses the same way as
   ``brahma::MPI``. See "Supported MPI" in the README for the families.

``brahma::HDF5`` (``brahma/interface/hdf5.h``)
   Intercepts the HDF5 C API (file, dataset, group, attribute operations such
   as ``H5Fcreate``, ``H5Fopen``, ``H5Fclose``, ``H5Dread``, ``H5Dwrite``,
   and many more). Available when built with ``BRAHMA_BUILD_WITH_HDF5``.

``brahma::InterfaceUtility`` (``brahma/interface/interface_utility.h``)
   Shared helper used by the interfaces to track which file descriptors /
   handles a tool is currently tracing.
