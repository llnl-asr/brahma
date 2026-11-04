
add_executable(test_posix test_posix.cpp)
add_dependencies(test_posix ${PROJECT_NAME})
target_link_libraries(test_posix ${PROJECT_NAME} rt)
# Compilers fold trivial calls in the printf family into each other as
# builtin optimizations: printf("literal\n") -> puts("literal"),
# fprintf(stdout, "format") -> printf("format"), snprintf(...) -> sprintf(...)
# when they can prove it's equivalent (e.g. empty/constant format strings).
# Since this test intercepts the entire printf/scanf family and logs via
# printf()/puts() itself, those foldings would recurse back through (or
# silently swap) the very interceptors under test. Disable all builtin
# recognition for this translation unit so every call reaches the symbol
# actually written in the source.
target_compile_options(test_posix PRIVATE -fno-builtin)
# The test calls every function with null arguments. With optimization glibc
# inlines getc/putc/getchar and friends, which read the FILE directly and crash
# on null before any interceptor runs, so build this test unoptimized. Its
# checks are assert()s, which release builds compile out; keep them.
target_compile_options(test_posix PRIVATE -O0 -UNDEBUG)
add_test(NAME test_posix COMMAND $<TARGET_FILE:test_posix>)

if(BRAHMA_BUILD_WITH_MPI)
    add_executable(test_mpiio test_mpiio.cpp)
    add_dependencies(test_mpiio ${PROJECT_NAME})
    target_link_libraries(test_mpiio ${PROJECT_NAME})
    add_test(NAME test_mpiio COMMAND $<TARGET_FILE:test_mpiio>)
    add_executable(test_mpi test_mpi.cpp)
    add_dependencies(test_mpi ${PROJECT_NAME})
    target_link_libraries(test_mpi ${PROJECT_NAME})
    add_test(NAME test_mpi COMMAND $<TARGET_FILE:test_mpi>)
    add_executable(test_mpi_compat_select test_mpi_compat_select.cpp)
    add_dependencies(test_mpi_compat_select ${PROJECT_NAME})
    target_link_libraries(test_mpi_compat_select ${PROJECT_NAME})
    add_test(NAME test_mpi_compat_select COMMAND $<TARGET_FILE:test_mpi_compat_select>)
    # One test per generated compat interface. A compat test checks only its
    # own implementation and major.minor; on any other MPI it exits 77 and is
    # reported as skipped (see below), never as a pass.
    file(GLOB BRAHMA_MPI_COMPAT_TESTS CONFIGURE_DEPENDS
            ${CMAKE_CURRENT_SOURCE_DIR}/test_mpi_*_compat.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/test_mpiio_*_compat.cpp)
    # The compat interface this build's MPI selects: implementation, major and
    # minor, the same rule the generated headers use.
    string(TOLOWER "${BRAHMA_MPI_IMPL_NAME}" _brahma_impl)
    set(_brahma_expected "")
    if(_brahma_impl AND BRAHMA_MPI_VERSION)
        math(EXPR _brahma_major "${BRAHMA_MPI_VERSION} / 100000")
        math(EXPR _brahma_minor "(${BRAHMA_MPI_VERSION} / 100) % 1000")
        set(_brahma_expected "${_brahma_impl}_${_brahma_major}_${_brahma_minor}")
    endif()
    message(STATUS "[${PROJECT_NAME}] MPI compat interface expected for this build: '${_brahma_expected}'")
    foreach(_t ${BRAHMA_MPI_COMPAT_TESTS})
        get_filename_component(_name ${_t} NAME_WE)
        add_executable(${_name} ${_t})
        add_dependencies(${_name} ${PROJECT_NAME})
        target_link_libraries(${_name} ${PROJECT_NAME})
        add_test(NAME ${_name} COMMAND $<TARGET_FILE:${_name}>)
        if(_brahma_expected AND _name MATCHES "^test_mpiio?_${_brahma_expected}_compat$")
            # This is the compat interface the build must test. It exits 77 if
            # it did not apply, and without SKIP_RETURN_CODE that is a failure,
            # so a family that silently stopped being selected cannot pass.
            set_tests_properties(${_name} PROPERTIES LABELS "mpi-compat-applies")
        else()
            # Another MPI's compat interface: nothing to check here. Report it
            # as skipped rather than passed.
            set_tests_properties(${_name} PROPERTIES SKIP_RETURN_CODE 77
                                 LABELS "mpi-compat-other")
        endif()
    endforeach()
endif()

if(BRAHMA_BUILD_WITH_HDF5)
    add_executable(test_hdf5 test_hdf5.cpp)
    add_dependencies(test_hdf5 ${PROJECT_NAME})
    target_link_libraries(test_hdf5 ${PROJECT_NAME})
    add_test(NAME test_hdf5 COMMAND $<TARGET_FILE:test_hdf5>)
endif()
