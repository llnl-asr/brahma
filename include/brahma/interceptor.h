//
// Created by hariharan on 8/8/22.
//

#ifndef BRAHMA_INTERCEPTOR_H
#define BRAHMA_INTERCEPTOR_H

#include <brahma/brahma_config.hpp>
/* Internal Headers */
#include <brahma/logging.h>
/* External Headers */
#include <gotcha/gotcha.h>

#include <cstdarg>
#include <memory>



// The unbinding points to the original function
#define GOTCHA_BINDING_MACRO_IMPL(fname, CLASS, SYMBOL)               \
  if constexpr (!std::is_same_v<decltype(&C::fname),                  \
                                decltype(&CLASS::fname)>) {           \
    gotcha_binding_t binding = {#fname, (void*)fname##_wrapper,       \
                                &fname##_brahma_handle};              \
    bindings.push_back(binding);                                      \
    fname##_fptr fn = &SYMBOL;                                        \
    if(fn){                                                           \
      gotcha_binding_t unbinding = {#fname, (void*)fn,                \
                                    &fname##_brahma_handle};          \
      unbindings.push_back(unbinding);                                \
      }                                                               \
}

#define GOTCHA_BINDING_MACRO(fname, CLASS) \
  GOTCHA_BINDING_MACRO_IMPL(fname, CLASS, ::fname)

// For functions declared with GOTCHA_MACRO_TYPEDEF_ALIAS.
#define GOTCHA_BINDING_MACRO_ALIAS(fname, CLASS) \
  GOTCHA_BINDING_MACRO_IMPL(fname, CLASS, fname##_symbol)

// For a "plain"/"64" function pair (e.g. stat/stat64, sendfile/sendfile64,
// freopen/freopen64): on platforms where glibc's _FILE_OFFSET_BITS=64
// __REDIRECT makes them the same underlying symbol renamed at the linker
// level (no independent "plain" dynamic symbol exists at all), binding both
// unconditionally would either fail the "plain" symbol's lookup outright or
// silently double-count calls to the one real symbol. Detect the aliasing
// at runtime via address comparison and only bind the "plain" name when
// it's actually independent of the "64" one -- the "64" one is always
// bound, since that's the name real calls resolve to either way.
#define GOTCHA_BINDING_MACRO_LFS64(name32, name64, CLASS)             \
  if ((void *)(name32##_fptr)&::name32 !=                             \
      (void *)(name64##_fptr)&::name64) {                             \
    GOTCHA_BINDING_MACRO(name32, CLASS);                               \
  }                                                                    \
  GOTCHA_BINDING_MACRO(name64, CLASS);

#define GOTCHA_MACRO_TYPEDEF(macroname, macroret, macroargs, macro2args_val, macroclass_name)         \
  typedef macroret(*macroname##_fptr) macroargs;                                                      \
  macroret __attribute__((weak)) macroname macroargs;                                                 \
  inline macroret macroname##_wrapper macroargs {                                                     \
    auto instance = macroclass_name::get_instance();                                                  \
    if (instance == nullptr) {                                                                        \
      macroname##_fptr fn = &::macroname;                                                             \
      return fn macro2args_val;                                                                       \
    }                                                                                                 \
    return instance->macroname macro2args_val;                                                        \
  }

// Same as GOTCHA_MACRO_TYPEDEF, but the weak declaration of the REAL
// external function is wrapped in extern "C". Needed for functions whose
// brahma-side signature was hand-corrected to a form that differs from
// whatever HDF5's own header declared earlier in this translation unit
// (e.g. HDF5's *_async functions under H5_DOXYGEN=1) -- without explicit
// extern "C", such a redeclaration is treated as a NEW C++ overload (with a
// mangled name) rather than unified with the real C-linkage exported
// symbol, so the weak symbol never resolves and calls jump through a NULL
// pointer at runtime. Use this variant for those functions specifically;
// GOTCHA_MACRO_TYPEDEF is unaffected and still used everywhere else.
// Same as GOTCHA_MACRO_TYPEDEF, for functions that glibc older than 2.33 only
// provides as inline wrappers around __xstat and friends (stat, lstat, fstat,
// fstatat, their 64 variants and mknod). Redeclaring one of those as a weak
// symbol makes every call to it in a translation unit that includes this
// header jump through a null address on such a glibc. The weak declaration
// goes under a private name linked to the real symbol with an asm label, so
// the application's own declaration is never touched.
#define GOTCHA_MACRO_TYPEDEF_ALIAS(macroname, macroret, macroargs, macro2args_val, macroclass_name)    \
  typedef macroret(*macroname##_fptr) macroargs;                                                      \
  extern "C" {                                                                                        \
  macroret macroname##_symbol macroargs __asm__(#macroname) __attribute__((weak));                    \
  }                                                                                                   \
  inline macroret macroname##_wrapper macroargs {                                                     \
    auto instance = macroclass_name::get_instance();                                                  \
    if (instance == nullptr) {                                                                        \
      macroname##_fptr fn = &macroname##_symbol;                                                      \
      return fn macro2args_val;                                                                       \
    }                                                                                                 \
    return instance->macroname macro2args_val;                                                        \
  }

#define GOTCHA_MACRO_TYPEDEF_C(macroname, macroret, macroargs, macro2args_val, macroclass_name)       \
  typedef macroret(*macroname##_fptr) macroargs;                                                      \
  extern "C" { macroret __attribute__((weak)) macroname macroargs; }                                  \
  inline macroret macroname##_wrapper macroargs {                                                     \
    auto instance = macroclass_name::get_instance();                                                  \
    if (instance == nullptr) {                                                                        \
      macroname##_fptr fn = &::macroname;                                                             \
      return fn macro2args_val;                                                                       \
    }                                                                                                 \
    return instance->macroname macro2args_val;                                                        \
  }

#define GOTCHA_MACRO_TYPEDEF_NOWEAK(macroname, macroret, macroargs, macro2args_val, macroclass_name)  \
  typedef macroret(*macroname##_fptr) macroargs;                                                      \
  macroret macroname macroargs;                                                                        \
  inline macroret macroname##_wrapper macroargs {                                                      \
    auto instance = macroclass_name::get_instance();                                                   \
    if (instance == nullptr) {                                                                         \
      macroname##_fptr fn = &::macroname;                                                              \
      return fn macro2args_val;                                                                        \
    }                                                                                                  \
    return instance->macroname macro2args_val;                                                         \
  }
  
#define GOTCHA_MACRO_TYPEDEF_OPEN(name, ret, args, args_val, start, \
                                  class_name)                       \
  typedef ret(*name##_fptr) args;                                   \
  ret __attribute__((weak)) name args;                              \
  inline ret name##_wrapper args {                                  \
    va_list _args;                                                  \
    va_start(_args, start);                                         \
    int mode = va_arg(_args, int);                                  \
    va_end(_args);                                                  \
    auto instance = class_name::get_instance();                     \
    if (instance == nullptr) {                                      \
      name##_fptr fn = &::name;                                     \
      return fn args_val;                                           \
    }                                                               \
    return instance->name args_val;                                 \
  }                                                                 

#define GOTCHA_MACRO_TYPEDEF_EXECL(name, ret, args, args_val, start, \
                                   class_name)                       \
  typedef ret(*name##_fptr) args;                                    \
  ret __attribute__((weak)) name args;                               \
  inline ret name##_wrapper args {                                   \
    va_list _args;                                                   \
    va_start(_args, start);                                          \
    char* val = va_arg(_args, char*);                                \
    va_end(_args);                                                   \
    auto instance = class_name::get_instance();                      \
    if (instance == nullptr) {                                       \
      return ::name args_val;                                        \
    }                                                                \
    return instance->name args_val;                                  \
  }                                                                  

// For variadic printf/scanf-family functions (format-string + "..."). The
// wrapper captures the varargs into a va_list and forwards to the matching
// v-prefixed function (e.g. vfprintf) directly -- we cannot call the
// original wrappee (a variadic function pointer) with a va_list, so instead
// of routing through gotcha_get_wrappee, the passthrough goes straight to
// the real v-function, exactly like glibc's own fprintf()->vfprintf().
#define GOTCHA_MACRO_TYPEDEF_VARFMT(name, vname, ret, args, vargs, fmtarg, \
                                    class_name)                        \
  typedef ret(*name##_fptr) args;                                      \
  ret __attribute__((weak)) name args;                                 \
  inline ret name##_wrapper args {                                     \
    va_list _args;                                                     \
    va_start(_args, fmtarg);                                           \
    auto instance = class_name::get_instance();                        \
    ret _r;                                                            \
    if (instance == nullptr) {                                         \
      _r = vname vargs;                                                \
    } else {                                                           \
      _r = instance->name vargs;                                       \
    }                                                                  \
    va_end(_args);                                                     \
    return _r;                                                         \
  }

// Same as GOTCHA_MACRO_TYPEDEF_VARFMT, but the weak declaration of the REAL
// external function is wrapped in extern "C" -- see GOTCHA_MACRO_TYPEDEF_C
// above for the full rationale. Needed for symbols like glibc's
// __isoc23_fscanf/scanf/sscanf family: no public header declares them (they
// are compiler-generated ISO-C23 redirect targets, glibc >= 2.38), so
// without explicit extern "C" our own weak declaration is the only one in
// the translation unit and gets C++ name-mangled, meaning application
// calls (compiled against the real, unmangled, C-linkage exported symbol)
// never reach it -- it stays an unresolved weak symbol (address 0), and
// calling it segfaults.
#define GOTCHA_MACRO_TYPEDEF_VARFMT_C(name, vname, ret, args, vargs,     \
                                      fmtarg, class_name)                \
  typedef ret(*name##_fptr) args;                                       \
  extern "C" { ret __attribute__((weak)) name args; }                   \
  inline ret name##_wrapper args {                                      \
    va_list _args;                                                      \
    va_start(_args, fmtarg);                                            \
    auto instance = class_name::get_instance();                        \
    ret _r;                                                            \
    if (instance == nullptr) {                                         \
      _r = vname vargs;                                                \
    } else {                                                           \
      _r = instance->name vargs;                                       \
    }                                                                  \
    va_end(_args);                                                     \
    return _r;                                                         \
  }

#define GOTCHA_MACRO_VAR(name) gotcha_wrappee_handle_t name##_brahma_handle;

#define BRAHMA_WRAPPER(name) name##_wrapper;

#define BRAHMA_UNWRAPPED_FUNC(macroname, ret, macroargs)                                \
  BRAHMA_LOG_INFO("[BRAHMA]\tFunction %s() not wrapped. Calling Original.\n", \
                  #macroname);                                                     \
  macroname##_fptr macroname##_wrappee =                                                \
      (macroname##_fptr)gotcha_get_wrappee(macroname##_brahma_handle);                  \
  ret result = macroname##_wrappee macroargs;

#define BRAHMA_UNWRAPPED_FUNC_VOID(macroname, macroargs)                                \
  BRAHMA_LOG_INFO("[BRAHMA]\tFunction %s() not wrapped. Calling Original.\n", \
                  #macroname);                                                     \
  macroname##_fptr macroname##_wrappee =                                                \
      (macroname##_fptr)gotcha_get_wrappee(macroname##_brahma_handle);                  \
  macroname##_wrappee macroargs;
#define BRAHMA_MAP_OR_FAIL(func_)                                      \
  auto __real_##func_ =                                                \
      (func_##_fptr)gotcha_get_wrappee(func_##_brahma_handle); \
  assert(__real_##func_ != NULL)

#endif  // BRAHMA_INTERCEPTOR_H
