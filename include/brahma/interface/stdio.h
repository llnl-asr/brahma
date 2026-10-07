//
// Created by hariharan on 8/8/22.
//

#ifndef BRAHMA_STDIO_H
#define BRAHMA_STDIO_H

#include <brahma/brahma_config.hpp>
/* Internal Headers */
#include <brahma/interceptor.h>
#include <brahma/interface/interface.h>
/* External Headers */
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <features.h>
#include <stdexcept>

namespace brahma {
class STDIO : public Interface {
 private:
  static std::shared_ptr<STDIO> my_instance;

 public:
  static std::shared_ptr<STDIO> get_instance() {
    if (my_instance == nullptr) {
      BRAHMA_LOG_INFO("STDIO class not intercepted but used", "");
      my_instance = std::make_shared<STDIO>();
    }
    return my_instance;
  }
  STDIO() : Interface() {}
  virtual ~STDIO(){};
  static int set_instance(std::shared_ptr<STDIO> instance_i) {
    if (instance_i != nullptr) {
      my_instance = instance_i;
      return 0;
    } else {
      BRAHMA_LOG_ERROR("%s instance_i is not set", "STDIO");
      throw std::runtime_error("instance_i is not set");
    }
  }

  template <typename C>
  size_t bind(const char *name, uint16_t priority);


  size_t unbind();

  virtual FILE *fopen(const char *path, const char *mode);
  virtual FILE *fopen64(const char *path, const char *mode);
  virtual int fclose(FILE *fp);
  virtual size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
  virtual size_t fwrite(const void *ptr, size_t size, size_t nmemb,
                        FILE *stream);
  virtual long ftell(FILE *fp);
  virtual int fseek(FILE *stream, long offset, int whence);
  virtual FILE* fdopen(int fd, const char *mode);
  virtual int fileno(FILE *stream);
  virtual FILE* tmpfile(void);
  virtual FILE* tmpfile64(void);
  virtual int fseeko(FILE *stream, off_t offset, int whence);
  virtual int fseeko64(FILE *stream, off64_t offset, int whence);
  virtual off_t ftello(FILE *stream);
  virtual off64_t ftello64(FILE *stream);

  virtual void clearerr(FILE *);
  virtual int feof(FILE *);
  virtual int ferror(FILE *);
  virtual int fflush(FILE *);
  virtual int fgetc(FILE *);
  virtual int fgetpos(FILE *, fpos_t *);
  virtual int fgetpos64(FILE *, fpos64_t *);
  virtual char* fgets(char *, int, FILE *);
  virtual void flockfile(FILE *);
  virtual int fputc(int, FILE *);
  virtual int fputs(const char *, FILE *);
  virtual FILE* freopen(const char *, const char *, FILE *);
  virtual FILE* freopen64(const char *, const char *, FILE *);
  virtual int fsetpos(FILE *, const fpos_t *);
  virtual int fsetpos64(FILE *, const fpos64_t *);
  virtual int ftrylockfile(FILE *);
  virtual void funlockfile(FILE *);
  virtual int getc(FILE *);
  virtual int getc_unlocked(FILE *);
  virtual int getw(FILE *);
  virtual int pclose(FILE *);
  virtual int putw(int, FILE *);
  virtual void rewind(FILE *);
  virtual int setvbuf(FILE *, char *, int, size_t);
  virtual int ungetc(int, FILE *);

  virtual int fprintf(FILE *stream, const char *format, va_list args);
  virtual int printf(const char *format, va_list args);
  virtual int sprintf(char *str, const char *format, va_list args);
  virtual int snprintf(char *str, size_t size, const char *format,
                       va_list args);
  virtual int vfprintf(FILE *stream, const char *format, va_list args);
  virtual int vprintf(const char *format, va_list args);
  virtual int vsprintf(char *str, const char *format, va_list args);
  virtual int vsnprintf(char *str, size_t size, const char *format,
                        va_list args);
  virtual int fscanf(FILE *stream, const char *format, va_list args);
  virtual int scanf(const char *format, va_list args);
  virtual int sscanf(const char *str, const char *format, va_list args);
  virtual int vfscanf(FILE *stream, const char *format, va_list args);
  virtual int vscanf(const char *format, va_list args);
  virtual int vsscanf(const char *str, const char *format, va_list args);
  // glibc >= 2.38 redirects the scanf family to these ISO-C23-compliant
  // aliases at the source level (a GCC _FORTIFY_SOURCE feature, distinct
  // from the older _FILE_OFFSET_BITS=64 "64"-suffix redirects), so
  // application calls to fscanf/scanf/etc. resolve directly to these
  // instead on such platforms; both must be bound to guarantee
  // interception either way. Before 2.38 these names are absent and binding
  // them is a no-op, so they stay unguarded to keep a wheel built on an old
  // glibc working on a newer one. The __isoc99_* names are the same redirect
  // for C99 mode on every glibc since 2.7.
  virtual int __isoc23_fscanf(FILE *stream, const char *format,
                              va_list args);
  virtual int __isoc23_scanf(const char *format, va_list args);
  virtual int __isoc23_sscanf(const char *str, const char *format,
                              va_list args);
  virtual int __isoc23_vfscanf(FILE *stream, const char *format,
                               va_list args);
  virtual int __isoc23_vscanf(const char *format, va_list args);
  virtual int __isoc23_vsscanf(const char *str, const char *format,
                               va_list args);
  virtual int __isoc99_fscanf(FILE *stream, const char *format,
                              va_list args);
  virtual int __isoc99_scanf(const char *format, va_list args);
  virtual int __isoc99_sscanf(const char *str, const char *format,
                              va_list args);
  virtual int __isoc99_vfscanf(FILE *stream, const char *format,
                               va_list args);
  virtual int __isoc99_vscanf(const char *format, va_list args);
  virtual int __isoc99_vsscanf(const char *str, const char *format,
                               va_list args);
  virtual size_t __fread_chk(void *ptr, size_t ptrlen, size_t size, size_t n, FILE *stream);
  virtual size_t __fread_unlocked_chk(void *ptr, size_t ptrlen, size_t size, size_t n, FILE *stream);
  virtual char *__fgets_chk(char *s, size_t size, int n, FILE *stream);
  virtual char *__fgets_unlocked_chk(char *s, size_t size, int n, FILE *stream);
  virtual int puts(const char *s);
  virtual int putchar(int c);
  virtual int putc(int c, FILE *stream);
  virtual int putc_unlocked(int c, FILE *stream);
  virtual int getchar(void);
  virtual int getchar_unlocked(void);
  virtual void perror(const char *s);
  virtual void setbuf(FILE *stream, char *buf);
  virtual void setbuffer(FILE *stream, char *buf, size_t size);
  virtual void setlinebuf(FILE *stream);
  virtual ssize_t getline(char **lineptr, size_t *n, FILE *stream);
  virtual ssize_t getdelim(char **lineptr, size_t *n, int delim,
                           FILE *stream);
  virtual FILE *fmemopen(void *buf, size_t size, const char *mode);
  virtual FILE *open_memstream(char **ptr, size_t *sizeloc);
  virtual FILE *popen(const char *command, const char *type);
  virtual char *tmpnam(char *s);

  GOTCHA_MACRO_VAR(fopen)
  GOTCHA_MACRO_VAR(fopen64)
  GOTCHA_MACRO_VAR(fclose)
  GOTCHA_MACRO_VAR(fread)
  GOTCHA_MACRO_VAR(fwrite)
  GOTCHA_MACRO_VAR(ftell)
  GOTCHA_MACRO_VAR(fseek)
  GOTCHA_MACRO_VAR(fdopen)
  GOTCHA_MACRO_VAR(fileno)
  GOTCHA_MACRO_VAR(tmpfile)
  GOTCHA_MACRO_VAR(tmpfile64)
  GOTCHA_MACRO_VAR(fseeko)
  GOTCHA_MACRO_VAR(fseeko64)
  GOTCHA_MACRO_VAR(ftello)
  GOTCHA_MACRO_VAR(ftello64)

  GOTCHA_MACRO_VAR(clearerr)
  GOTCHA_MACRO_VAR(feof)
  GOTCHA_MACRO_VAR(ferror)
  GOTCHA_MACRO_VAR(fflush)
  GOTCHA_MACRO_VAR(fgetc)
  GOTCHA_MACRO_VAR(fgetpos)
  GOTCHA_MACRO_VAR(fgetpos64)
  GOTCHA_MACRO_VAR(fgets)
  GOTCHA_MACRO_VAR(flockfile)
  GOTCHA_MACRO_VAR(fputc)
  GOTCHA_MACRO_VAR(fputs)
  GOTCHA_MACRO_VAR(freopen)
  GOTCHA_MACRO_VAR(freopen64)
  GOTCHA_MACRO_VAR(fsetpos)
  GOTCHA_MACRO_VAR(fsetpos64)
  GOTCHA_MACRO_VAR(ftrylockfile)
  GOTCHA_MACRO_VAR(funlockfile)
  GOTCHA_MACRO_VAR(getc)
  GOTCHA_MACRO_VAR(getc_unlocked)
  GOTCHA_MACRO_VAR(getw)
  GOTCHA_MACRO_VAR(pclose)
  GOTCHA_MACRO_VAR(putw)
  GOTCHA_MACRO_VAR(rewind)
  GOTCHA_MACRO_VAR(setvbuf)
  GOTCHA_MACRO_VAR(ungetc)
  GOTCHA_MACRO_VAR(fprintf)
  GOTCHA_MACRO_VAR(printf)
  GOTCHA_MACRO_VAR(sprintf)
  GOTCHA_MACRO_VAR(snprintf)
  GOTCHA_MACRO_VAR(vfprintf)
  GOTCHA_MACRO_VAR(vprintf)
  GOTCHA_MACRO_VAR(vsprintf)
  GOTCHA_MACRO_VAR(vsnprintf)
  GOTCHA_MACRO_VAR(fscanf)
  GOTCHA_MACRO_VAR(scanf)
  GOTCHA_MACRO_VAR(sscanf)
  GOTCHA_MACRO_VAR(vfscanf)
  GOTCHA_MACRO_VAR(vscanf)
  GOTCHA_MACRO_VAR(vsscanf)
  GOTCHA_MACRO_VAR(__isoc23_fscanf)
  GOTCHA_MACRO_VAR(__isoc23_scanf)
  GOTCHA_MACRO_VAR(__isoc23_sscanf)
  GOTCHA_MACRO_VAR(__isoc23_vfscanf)
  GOTCHA_MACRO_VAR(__isoc23_vscanf)
  GOTCHA_MACRO_VAR(__isoc23_vsscanf)
  GOTCHA_MACRO_VAR(__isoc99_fscanf)
  GOTCHA_MACRO_VAR(__isoc99_scanf)
  GOTCHA_MACRO_VAR(__isoc99_sscanf)
  GOTCHA_MACRO_VAR(__isoc99_vfscanf)
  GOTCHA_MACRO_VAR(__isoc99_vscanf)
  GOTCHA_MACRO_VAR(__isoc99_vsscanf)
  GOTCHA_MACRO_VAR(__fread_chk)
  GOTCHA_MACRO_VAR(__fread_unlocked_chk)
  GOTCHA_MACRO_VAR(__fgets_chk)
  GOTCHA_MACRO_VAR(__fgets_unlocked_chk)
  GOTCHA_MACRO_VAR(puts)
  GOTCHA_MACRO_VAR(putchar)
  GOTCHA_MACRO_VAR(putc)
  GOTCHA_MACRO_VAR(putc_unlocked)
  GOTCHA_MACRO_VAR(getchar)
  GOTCHA_MACRO_VAR(getchar_unlocked)
  GOTCHA_MACRO_VAR(perror)
  GOTCHA_MACRO_VAR(setbuf)
  GOTCHA_MACRO_VAR(setbuffer)
  GOTCHA_MACRO_VAR(setlinebuf)
  GOTCHA_MACRO_VAR(getline)
  GOTCHA_MACRO_VAR(getdelim)
  GOTCHA_MACRO_VAR(fmemopen)
  GOTCHA_MACRO_VAR(open_memstream)
  GOTCHA_MACRO_VAR(popen)
  GOTCHA_MACRO_VAR(tmpnam)

};

}  // namespace brahma
GOTCHA_MACRO_TYPEDEF(fopen, FILE *, (const char *path, const char *mode),
                     (path, mode), brahma::STDIO)

GOTCHA_MACRO_TYPEDEF(fopen64, FILE *, (const char *path, const char *mode),
                     (path, mode), brahma::STDIO)

GOTCHA_MACRO_TYPEDEF(fclose, int, (FILE * fp), (fp), brahma::STDIO)

GOTCHA_MACRO_TYPEDEF(fread, size_t,
                     (void *ptr, size_t size, size_t nmemb, FILE *stream),
                     (ptr, size, nmemb, stream), brahma::STDIO)

GOTCHA_MACRO_TYPEDEF(fwrite, size_t,
                     (const void *ptr, size_t size, size_t nmemb, FILE *stream),
                     (ptr, size, nmemb, stream), brahma::STDIO)

GOTCHA_MACRO_TYPEDEF(ftell, long, (FILE * stream), (stream), brahma::STDIO)

GOTCHA_MACRO_TYPEDEF(fseek, int, (FILE * stream, long offset, int whence),
                     (stream, offset, whence), brahma::STDIO)
GOTCHA_MACRO_TYPEDEF(fdopen, FILE *, (int fd, const char *mode), (fd, mode),
                     brahma::STDIO)
GOTCHA_MACRO_TYPEDEF(fileno, int, (FILE * stream), (stream), brahma::STDIO)
GOTCHA_MACRO_TYPEDEF(tmpfile, FILE *, (void), (), brahma::STDIO)
GOTCHA_MACRO_TYPEDEF(tmpfile64, FILE *, (void), (), brahma::STDIO)
GOTCHA_MACRO_TYPEDEF(fseeko, int, (FILE * stream, off_t offset, int whence),
                     (stream, offset, whence), brahma::STDIO)
GOTCHA_MACRO_TYPEDEF(fseeko64, int, (FILE * stream, off64_t offset, int whence),
                     (stream, offset, whence), brahma::STDIO)
GOTCHA_MACRO_TYPEDEF(ftello, off_t, (FILE * stream), (stream), brahma::STDIO)
GOTCHA_MACRO_TYPEDEF(ftello64, off64_t, (FILE * stream), (stream),
                     brahma::STDIO)

GOTCHA_MACRO_TYPEDEF(clearerr, void, (FILE * stream), (stream), brahma::STDIO)
GOTCHA_MACRO_TYPEDEF(feof, int, (FILE * stream), (stream), brahma::STDIO)
GOTCHA_MACRO_TYPEDEF(ferror, int, (FILE * stream), (stream), brahma::STDIO)
GOTCHA_MACRO_TYPEDEF(fflush, int, (FILE * stream), (stream), brahma::STDIO)
GOTCHA_MACRO_TYPEDEF(fgetc, int, (FILE * stream), (stream), brahma::STDIO)
GOTCHA_MACRO_TYPEDEF(fgetpos, int, (FILE * stream, fpos_t * pos), (stream, pos),
                     brahma::STDIO)
GOTCHA_MACRO_TYPEDEF(fgetpos64, int, (FILE * stream, fpos64_t * pos),
                     (stream, pos), brahma::STDIO)
GOTCHA_MACRO_TYPEDEF(fgets, char *, (char *s, int size, FILE * stream),
                     (s, size, stream), brahma::STDIO)
GOTCHA_MACRO_TYPEDEF(flockfile, void, (FILE * stream), (stream), brahma::STDIO)
GOTCHA_MACRO_TYPEDEF(fputc, int, (int c, FILE * stream), (c, stream),
                     brahma::STDIO)
GOTCHA_MACRO_TYPEDEF(fputs, int, (const char *s, FILE * stream), (s, stream),
                     brahma::STDIO)
GOTCHA_MACRO_TYPEDEF(freopen, FILE *, (const char *pathname, const char *mode,
                                      FILE * stream),
                     (pathname, mode, stream), brahma::STDIO)
GOTCHA_MACRO_TYPEDEF(freopen64, FILE *, (const char *pathname,
                                        const char *mode, FILE * stream),
                     (pathname, mode, stream), brahma::STDIO)
GOTCHA_MACRO_TYPEDEF(fsetpos, int, (FILE * stream, const fpos_t * pos),
                     (stream, pos), brahma::STDIO)
GOTCHA_MACRO_TYPEDEF(fsetpos64, int, (FILE * stream, const fpos64_t * pos),
                     (stream, pos), brahma::STDIO)
GOTCHA_MACRO_TYPEDEF(ftrylockfile, int, (FILE * stream), (stream), brahma::STDIO)
GOTCHA_MACRO_TYPEDEF(funlockfile, void, (FILE * stream), (stream), brahma::STDIO)
GOTCHA_MACRO_TYPEDEF(getc, int, (FILE * stream), (stream), brahma::STDIO)
GOTCHA_MACRO_TYPEDEF_NOWEAK(getc_unlocked, int, (FILE * stream), (stream), brahma::STDIO)
GOTCHA_MACRO_TYPEDEF(getw, int, (FILE * stream), (stream), brahma::STDIO)
GOTCHA_MACRO_TYPEDEF(pclose, int, (FILE * stream), (stream), brahma::STDIO)
GOTCHA_MACRO_TYPEDEF(putw, int, (int w, FILE * stream), (w, stream), brahma::STDIO)
GOTCHA_MACRO_TYPEDEF(rewind, void, (FILE * stream), (stream), brahma::STDIO)
GOTCHA_MACRO_TYPEDEF(setvbuf, int, (FILE * stream, char *buf, int mode,
                                    size_t size),
                     (stream, buf, mode, size), brahma::STDIO)
GOTCHA_MACRO_TYPEDEF(ungetc, int, (int c, FILE * stream), (c, stream), brahma::STDIO)

GOTCHA_MACRO_TYPEDEF_VARFMT(fprintf, vfprintf, int,
                            (FILE * stream, const char *format, ...),
                            (stream, format, _args), format, brahma::STDIO)
GOTCHA_MACRO_TYPEDEF_VARFMT(printf, vprintf, int, (const char *format, ...),
                            (format, _args), format, brahma::STDIO)
GOTCHA_MACRO_TYPEDEF_VARFMT(sprintf, vsprintf, int,
                            (char *str, const char *format, ...),
                            (str, format, _args), format, brahma::STDIO)
GOTCHA_MACRO_TYPEDEF_VARFMT(snprintf, vsnprintf, int,
                            (char *str, size_t size, const char *format, ...),
                            (str, size, format, _args), format, brahma::STDIO)
GOTCHA_MACRO_TYPEDEF(vfprintf, int,
                     (FILE * stream, const char *format, va_list args),
                     (stream, format, args), brahma::STDIO)
GOTCHA_MACRO_TYPEDEF(vprintf, int, (const char *format, va_list args),
                     (format, args), brahma::STDIO)
GOTCHA_MACRO_TYPEDEF(vsprintf, int,
                     (char *str, const char *format, va_list args),
                     (str, format, args), brahma::STDIO)
GOTCHA_MACRO_TYPEDEF(vsnprintf, int,
                     (char *str, size_t size, const char *format,
                      va_list args),
                     (str, size, format, args), brahma::STDIO)
GOTCHA_MACRO_TYPEDEF_VARFMT(fscanf, vfscanf, int,
                            (FILE * stream, const char *format, ...),
                            (stream, format, _args), format, brahma::STDIO)
GOTCHA_MACRO_TYPEDEF_VARFMT(scanf, vscanf, int, (const char *format, ...),
                            (format, _args), format, brahma::STDIO)
GOTCHA_MACRO_TYPEDEF_VARFMT(sscanf, vsscanf, int,
                            (const char *str, const char *format, ...),
                            (str, format, _args), format, brahma::STDIO)
GOTCHA_MACRO_TYPEDEF(vfscanf, int,
                     (FILE * stream, const char *format, va_list args),
                     (stream, format, args), brahma::STDIO)
GOTCHA_MACRO_TYPEDEF(vscanf, int, (const char *format, va_list args),
                     (format, args), brahma::STDIO)
GOTCHA_MACRO_TYPEDEF(vsscanf, int,
                     (const char *str, const char *format, va_list args),
                     (str, format, args), brahma::STDIO)
// The v-variants (vname) must be declared before the VARFMT macros below
// reference them: unlike vfscanf/vscanf/vsscanf (already declared via
// <cstdio>), these __isoc23_v* symbols aren't exposed by any public
// header, so our own weak declaration is the only one the compiler sees --
// which means it MUST be wrapped in extern "C" (the _C macro variants) or
// it gets C++ name-mangled and never resolves to the real, unmangled,
// C-linkage exported symbol (confirmed via a minimal gotcha-only repro:
// the mangled call site stays an unresolved weak symbol at address 0, and
// calling it segfaults).
GOTCHA_MACRO_TYPEDEF_C(__isoc23_vfscanf, int,
                       (FILE * stream, const char *format, va_list args),
                       (stream, format, args), brahma::STDIO)
GOTCHA_MACRO_TYPEDEF_C(__isoc23_vscanf, int,
                       (const char *format, va_list args),
                       (format, args), brahma::STDIO)
GOTCHA_MACRO_TYPEDEF_C(__isoc23_vsscanf, int,
                       (const char *str, const char *format, va_list args),
                       (str, format, args), brahma::STDIO)
GOTCHA_MACRO_TYPEDEF_VARFMT_C(__isoc23_fscanf, __isoc23_vfscanf, int,
                              (FILE * stream, const char *format, ...),
                              (stream, format, _args), format, brahma::STDIO)
GOTCHA_MACRO_TYPEDEF_VARFMT_C(__isoc23_scanf, __isoc23_vscanf, int,
                              (const char *format, ...),
                              (format, _args), format, brahma::STDIO)
GOTCHA_MACRO_TYPEDEF_VARFMT_C(__isoc23_sscanf, __isoc23_vsscanf, int,
                              (const char *str, const char *format, ...),
                              (str, format, _args), format, brahma::STDIO)
GOTCHA_MACRO_TYPEDEF_C(__isoc99_vfscanf, int,
                       (FILE * stream, const char *format, va_list args),
                       (stream, format, args), brahma::STDIO)
GOTCHA_MACRO_TYPEDEF_C(__isoc99_vscanf, int,
                       (const char *format, va_list args),
                       (format, args), brahma::STDIO)
GOTCHA_MACRO_TYPEDEF_C(__isoc99_vsscanf, int,
                       (const char *str, const char *format, va_list args),
                       (str, format, args), brahma::STDIO)
GOTCHA_MACRO_TYPEDEF_VARFMT_C(__isoc99_fscanf, __isoc99_vfscanf, int,
                              (FILE * stream, const char *format, ...),
                              (stream, format, _args), format, brahma::STDIO)
GOTCHA_MACRO_TYPEDEF_VARFMT_C(__isoc99_scanf, __isoc99_vscanf, int,
                              (const char *format, ...),
                              (format, _args), format, brahma::STDIO)
GOTCHA_MACRO_TYPEDEF_VARFMT_C(__isoc99_sscanf, __isoc99_vsscanf, int,
                              (const char *str, const char *format, ...),
                              (str, format, _args), format, brahma::STDIO)
GOTCHA_MACRO_TYPEDEF_C(__fread_chk, size_t, (void *ptr, size_t ptrlen, size_t size, size_t n, FILE *stream),
                       (ptr, ptrlen, size, n, stream), brahma::STDIO)
GOTCHA_MACRO_TYPEDEF_C(__fread_unlocked_chk, size_t, (void *ptr, size_t ptrlen, size_t size, size_t n, FILE *stream),
                       (ptr, ptrlen, size, n, stream), brahma::STDIO)
GOTCHA_MACRO_TYPEDEF_C(__fgets_chk, char *, (char *s, size_t size, int n, FILE *stream),
                       (s, size, n, stream), brahma::STDIO)
GOTCHA_MACRO_TYPEDEF_C(__fgets_unlocked_chk, char *, (char *s, size_t size, int n, FILE *stream),
                       (s, size, n, stream), brahma::STDIO)
GOTCHA_MACRO_TYPEDEF(puts, int, (const char *s), (s), brahma::STDIO)
GOTCHA_MACRO_TYPEDEF(putchar, int, (int c), (c), brahma::STDIO)
GOTCHA_MACRO_TYPEDEF(putc, int, (int c, FILE * stream), (c, stream),
                     brahma::STDIO)
GOTCHA_MACRO_TYPEDEF_NOWEAK(putc_unlocked, int, (int c, FILE * stream),
                            (c, stream), brahma::STDIO)
GOTCHA_MACRO_TYPEDEF(getchar, int, (void), (), brahma::STDIO)
GOTCHA_MACRO_TYPEDEF_NOWEAK(getchar_unlocked, int, (void), (), brahma::STDIO)
GOTCHA_MACRO_TYPEDEF(perror, void, (const char *s), (s), brahma::STDIO)
GOTCHA_MACRO_TYPEDEF(setbuf, void, (FILE * stream, char *buf), (stream, buf),
                     brahma::STDIO)
GOTCHA_MACRO_TYPEDEF(setbuffer, void,
                     (FILE * stream, char *buf, size_t size),
                     (stream, buf, size), brahma::STDIO)
GOTCHA_MACRO_TYPEDEF(setlinebuf, void, (FILE * stream), (stream),
                     brahma::STDIO)
GOTCHA_MACRO_TYPEDEF(getline, ssize_t,
                     (char **lineptr, size_t *n, FILE *stream),
                     (lineptr, n, stream), brahma::STDIO)
GOTCHA_MACRO_TYPEDEF(getdelim, ssize_t,
                     (char **lineptr, size_t *n, int delim, FILE *stream),
                     (lineptr, n, delim, stream), brahma::STDIO)
GOTCHA_MACRO_TYPEDEF(fmemopen, FILE *,
                     (void *buf, size_t size, const char *mode),
                     (buf, size, mode), brahma::STDIO)
GOTCHA_MACRO_TYPEDEF(open_memstream, FILE *,
                     (char **ptr, size_t *sizeloc), (ptr, sizeloc),
                     brahma::STDIO)
GOTCHA_MACRO_TYPEDEF(popen, FILE *,
                     (const char *command, const char *type),
                     (command, type), brahma::STDIO)
GOTCHA_MACRO_TYPEDEF(tmpnam, char *, (char *s), (s), brahma::STDIO)


template <typename C>
size_t brahma::STDIO::bind(const char *name, uint16_t priority) {
  GOTCHA_BINDING_MACRO(fopen, STDIO);
  GOTCHA_BINDING_MACRO(fopen64, STDIO);
  GOTCHA_BINDING_MACRO(fclose, STDIO);
  GOTCHA_BINDING_MACRO(fread, STDIO);
  GOTCHA_BINDING_MACRO(fwrite, STDIO);
  GOTCHA_BINDING_MACRO(ftell, STDIO);
  GOTCHA_BINDING_MACRO(fseek, STDIO);
  GOTCHA_BINDING_MACRO(tmpfile, STDIO);
  GOTCHA_BINDING_MACRO(tmpfile64, STDIO);
  GOTCHA_BINDING_MACRO(fseeko, STDIO);
  GOTCHA_BINDING_MACRO(fseeko64, STDIO);
  GOTCHA_BINDING_MACRO(ftello, STDIO);
  GOTCHA_BINDING_MACRO(ftello64, STDIO);
  GOTCHA_BINDING_MACRO(fdopen, STDIO);
  GOTCHA_BINDING_MACRO(fileno, STDIO);

  GOTCHA_BINDING_MACRO(clearerr, STDIO);
  GOTCHA_BINDING_MACRO(feof, STDIO);
  GOTCHA_BINDING_MACRO(ferror, STDIO);
  GOTCHA_BINDING_MACRO(fflush, STDIO);
  GOTCHA_BINDING_MACRO(fgetc, STDIO);
  GOTCHA_BINDING_MACRO(fgetpos, STDIO);
  GOTCHA_BINDING_MACRO(fgetpos64, STDIO);
  GOTCHA_BINDING_MACRO(fgets, STDIO);
  GOTCHA_BINDING_MACRO(flockfile, STDIO);
  GOTCHA_BINDING_MACRO(fputc, STDIO);
  GOTCHA_BINDING_MACRO(fputs, STDIO);
  // On some platforms, glibc's _FILE_OFFSET_BITS=64 __REDIRECT mechanism
  // makes "freopen" and "freopen64" the same underlying symbol renamed at
  // the linker level (no independent "freopen" dynamic symbol exists at
  // all), rather than two genuinely distinct functions. Binding both
  // unconditionally would either fail the "freopen" lookup outright or
  // silently double-count calls to the one real symbol. Detect the
  // aliasing at runtime via address comparison and only bind "freopen" when
  // it's actually independent of "freopen64".
  if ((void *)(freopen_fptr)&::freopen != (void *)(freopen64_fptr)&::freopen64) {
    GOTCHA_BINDING_MACRO(freopen, STDIO);
  }
  GOTCHA_BINDING_MACRO(freopen64, STDIO);
  GOTCHA_BINDING_MACRO(fsetpos, STDIO);
  GOTCHA_BINDING_MACRO(fsetpos64, STDIO);
  GOTCHA_BINDING_MACRO(ftrylockfile, STDIO);
  GOTCHA_BINDING_MACRO(funlockfile, STDIO);
  GOTCHA_BINDING_MACRO(getc, STDIO);
  GOTCHA_BINDING_MACRO(getc_unlocked, STDIO);
  GOTCHA_BINDING_MACRO(getw, STDIO);
  GOTCHA_BINDING_MACRO(pclose, STDIO);
  GOTCHA_BINDING_MACRO(putw, STDIO);
  GOTCHA_BINDING_MACRO(rewind, STDIO);
  GOTCHA_BINDING_MACRO(setvbuf, STDIO);
  GOTCHA_BINDING_MACRO(ungetc, STDIO);

  GOTCHA_BINDING_MACRO(fprintf, STDIO);
  GOTCHA_BINDING_MACRO(printf, STDIO);
  GOTCHA_BINDING_MACRO(sprintf, STDIO);
  GOTCHA_BINDING_MACRO(snprintf, STDIO);
  GOTCHA_BINDING_MACRO(vfprintf, STDIO);
  GOTCHA_BINDING_MACRO(vprintf, STDIO);
  GOTCHA_BINDING_MACRO(vsprintf, STDIO);
  GOTCHA_BINDING_MACRO(vsnprintf, STDIO);
  GOTCHA_BINDING_MACRO(fscanf, STDIO);
  GOTCHA_BINDING_MACRO(scanf, STDIO);
  GOTCHA_BINDING_MACRO(sscanf, STDIO);
  GOTCHA_BINDING_MACRO(vfscanf, STDIO);
  GOTCHA_BINDING_MACRO(vscanf, STDIO);
  GOTCHA_BINDING_MACRO(vsscanf, STDIO);
  GOTCHA_BINDING_MACRO(__isoc23_fscanf, STDIO);
  GOTCHA_BINDING_MACRO(__isoc23_scanf, STDIO);
  GOTCHA_BINDING_MACRO(__isoc23_sscanf, STDIO);
  GOTCHA_BINDING_MACRO(__isoc23_vfscanf, STDIO);
  GOTCHA_BINDING_MACRO(__isoc23_vscanf, STDIO);
  GOTCHA_BINDING_MACRO(__isoc23_vsscanf, STDIO);
  GOTCHA_BINDING_MACRO(__isoc99_fscanf, STDIO);
  GOTCHA_BINDING_MACRO(__isoc99_scanf, STDIO);
  GOTCHA_BINDING_MACRO(__isoc99_sscanf, STDIO);
  GOTCHA_BINDING_MACRO(__isoc99_vfscanf, STDIO);
  GOTCHA_BINDING_MACRO(__isoc99_vscanf, STDIO);
  GOTCHA_BINDING_MACRO(__isoc99_vsscanf, STDIO);
  GOTCHA_BINDING_MACRO(__fread_chk, STDIO);
  GOTCHA_BINDING_MACRO(__fread_unlocked_chk, STDIO);
  GOTCHA_BINDING_MACRO(__fgets_chk, STDIO);
  GOTCHA_BINDING_MACRO(__fgets_unlocked_chk, STDIO);
  GOTCHA_BINDING_MACRO(puts, STDIO);
  GOTCHA_BINDING_MACRO(putchar, STDIO);
  GOTCHA_BINDING_MACRO(putc, STDIO);
  GOTCHA_BINDING_MACRO(putc_unlocked, STDIO);
  GOTCHA_BINDING_MACRO(getchar, STDIO);
  GOTCHA_BINDING_MACRO(getchar_unlocked, STDIO);
  GOTCHA_BINDING_MACRO(perror, STDIO);
  GOTCHA_BINDING_MACRO(setbuf, STDIO);
  GOTCHA_BINDING_MACRO(setbuffer, STDIO);
  GOTCHA_BINDING_MACRO(setlinebuf, STDIO);
  GOTCHA_BINDING_MACRO(getline, STDIO);
  GOTCHA_BINDING_MACRO(getdelim, STDIO);
  GOTCHA_BINDING_MACRO(fmemopen, STDIO);
  GOTCHA_BINDING_MACRO(open_memstream, STDIO);
  GOTCHA_BINDING_MACRO(popen, STDIO);
  GOTCHA_BINDING_MACRO(tmpnam, STDIO);

  num_bindings = bindings.size();
  if (num_bindings > 0) {
    // sprintf/snprintf are themselves interceptable STDIO functions;
    // using them here would self-trigger whatever override is active for
    // them. strcpy/strcat are never bound, so they're safe for this
    // internal bookkeeping.
    strcpy(tool_name, name);
    strcat(tool_name, "_stdio");
    gotcha_binding_t *raw_bindings = bindings.data();
    gotcha_wrap(raw_bindings, num_bindings, tool_name);
    bind_priority = priority;
    gotcha_set_priority(tool_name, priority);
  }
  return num_bindings;
}
#endif  // BRAHMA_STDIO_H
