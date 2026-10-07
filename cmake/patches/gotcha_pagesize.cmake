# GOTCHA 1.0.10 builds with -D_POSIX_C_SOURCE=200809L and calls getpagesize(),
# which glibc 2.34 and newer no longer declare in that mode; gcc 14 rejects the
# implicit declaration. The function is still exported, so declare it. Run by
# FetchContent in GOTCHA's source directory; does nothing once the macro no
# longer matches.
set(_file src/libc_wrappers.h)
if(NOT EXISTS "${_file}")
  message(WARNING "gotcha patch: ${_file} not found in ${CMAKE_CURRENT_SOURCE_DIR}")
  return()
endif()
file(READ "${_file}" _content)
if(NOT _content MATCHES "extern int getpagesize")
  string(REPLACE "#define gotcha_getpagesize getpagesize"
         "extern int getpagesize(void);\n#define gotcha_getpagesize getpagesize"
         _content "${_content}")
  file(WRITE "${_file}" "${_content}")
endif()
