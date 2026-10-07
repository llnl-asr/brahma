include(brahma-utils)
if (BRAHMA_LOGGER STREQUAL "CPP_LOGGER")
    fetch_package(NAME cpp-logger cpp_logger CPP_LOGGER Cpp_logger
                  VERSION 2.0.0
                  GIT https://github.com/llnl-asr/cpp-logger.git
                  TAG v0.0.8
                  #SHALLOW
                  #DIAG
                  VARS "CPP_LOGGER_ENABLE_TESTING OFF BOOL"
                       "CPP_LOGGER_LIBDIR_AS_LIB ${BRAHMA_LIBDIR_AS_LIB} BOOL")

    # Propagate up one more level to top-level CMakeLists.txt
    set(cpp-logger_POPULATED    "${cpp-logger_POPULATED}"    PARENT_SCOPE)
    set(cpp-logger_INCLUDE_DIRS "${cpp-logger_INCLUDE_DIRS}" PARENT_SCOPE)
    set(cpp-logger_LIBRARIES    "${cpp-logger_LIBRARIES}"    PARENT_SCOPE)
    set(cpp-logger_FOUND        "${cpp-logger_FOUND}"        PARENT_SCOPE)
endif()

fetch_package(NAME gotcha GOTCHA Gotcha
              VERSION 2.4.4
              GIT https://github.com/LLNL/GOTCHA.git
              TAG v1.0.10
              REQUIRED
              PATCH_COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/../cmake/patches/gotcha_pagesize.cmake
              #DIAG
              VARS "GOTCHA_ENABLE_TESTING OFF BOOL"
                   "CMAKE_INSTALL_LIBDIR ${BRAHMA_LIBDIR} PATH")


# Propagate up one more level to top-level CMakeLists.txt
set(gotcha_POPULATED    "${gotcha_POPULATED}"    PARENT_SCOPE)
set(gotcha_INCLUDE_DIRS "${gotcha_INCLUDE_DIRS}" PARENT_SCOPE)
set(gotcha_LIBRARIES    "${gotcha_LIBRARIES}"    PARENT_SCOPE)
set(gotcha_FOUND        "${gotcha_FOUND}"        PARENT_SCOPE)


get_target_property(_gotcha_imported gotcha IMPORTED)
if (NOT _gotcha_imported OR _gotcha_imported MATCHES "NOTFOUND")
    # fetch_package() is a CMake function, so FetchContent_GetProperties()
    # called inside it sets gotcha_SOURCE_DIR / gotcha_BINARY_DIR only in
    # the function's local scope.  Re-fetch the properties here so the
    # variables are available when we call target_include_directories below.
    if (gotcha_POPULATED)
        include(FetchContent)
        FetchContent_GetProperties(gotcha)
    endif()
    # GOTCHA uses GNU-specific extensions. So it needs to be
    # compiled with -std=gnu11 instead of -std=c11.
    set_target_properties(gotcha PROPERTIES C_EXTENSIONS ON)
    # Setting gotcha include dirs – must include the binary dir because
    # gotcha_config.h is generated there (not present in the source tree).
    target_include_directories(gotcha PUBLIC
        $<BUILD_INTERFACE:${gotcha_BINARY_DIR}/include>
        $<BUILD_INTERFACE:${gotcha_SOURCE_DIR}/include>
    )
endif ()
