
add_executable(test_posix test_posix.cpp)
add_dependencies(test_posix ${PROJECT_NAME})
target_link_libraries(test_posix ${PROJECT_NAME})
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
endif()

if(BRAHMA_BUILD_WITH_HDF5)
    add_executable(test_hdf5 test_hdf5.cpp)
    add_dependencies(test_hdf5 ${PROJECT_NAME})
    target_link_libraries(test_hdf5 ${PROJECT_NAME})
    add_test(NAME test_hdf5 COMMAND $<TARGET_FILE:test_hdf5>)
endif()
