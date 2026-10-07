//
// Created by hariharan on 8/16/22.
//
#include <brahma/interface/stdio.h>
#include <brahma/interface/stdio_bypass.h>

namespace brahma {

std::shared_ptr<STDIO> STDIO::my_instance = nullptr;
FILE *STDIO::fopen(const char *path, const char *mode) {
  BRAHMA_UNWRAPPED_FUNC(fopen, FILE *, (path, mode));
  return result;
}

FILE *STDIO::fopen64(const char *path, const char *mode) {
  BRAHMA_UNWRAPPED_FUNC(fopen64, FILE *, (path, mode));
  return result;
}

int STDIO::fclose(FILE *fp) {
  BRAHMA_UNWRAPPED_FUNC(fclose, int, (fp));
  return result;
}

size_t STDIO::fread(void *ptr, size_t size, size_t nmemb, FILE *stream) {
  BRAHMA_UNWRAPPED_FUNC(fread, size_t, (ptr, size, nmemb, stream));
  return result;
}

size_t STDIO::fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream) {
  BRAHMA_UNWRAPPED_FUNC(fwrite, size_t, (ptr, size, nmemb, stream));
  return result;
}

long STDIO::ftell(FILE *stream) {
  BRAHMA_UNWRAPPED_FUNC(ftell, long, (stream));
  return result;
}

int STDIO::fseek(FILE *stream, long offset, int whence) {
  BRAHMA_UNWRAPPED_FUNC(fseek, int, (stream, offset, whence));
  return result;
}
FILE *STDIO::fdopen(int fd, const char *mode) {
  BRAHMA_UNWRAPPED_FUNC(fdopen, FILE *, (fd, mode));
  return result;
}
int STDIO::fileno(FILE *stream) {
  BRAHMA_UNWRAPPED_FUNC(fileno, int, (stream));
  return result;
}
FILE *STDIO::tmpfile(void) {
  BRAHMA_UNWRAPPED_FUNC(tmpfile, FILE *, ());
  return result;
}
FILE *STDIO::tmpfile64(void) {
  BRAHMA_UNWRAPPED_FUNC(tmpfile64, FILE *, ());
  return result;
}
int STDIO::fseeko(FILE *stream, off_t offset, int whence) {
  BRAHMA_UNWRAPPED_FUNC(fseeko, int, (stream, offset, whence));
  return result;
}
int STDIO::fseeko64(FILE *stream, off64_t offset, int whence) {
  BRAHMA_UNWRAPPED_FUNC(fseeko64, int, (stream, offset, whence));
  return result;
}
off_t STDIO::ftello(FILE *stream) {
  BRAHMA_UNWRAPPED_FUNC(ftello, off_t, (stream));
  return result;
}
off64_t STDIO::ftello64(FILE *stream) {
  BRAHMA_UNWRAPPED_FUNC(ftello64, off64_t, (stream));
  return result;
}


void STDIO::clearerr(FILE *stream) {
  BRAHMA_UNWRAPPED_FUNC_VOID(clearerr, (stream));
}
int STDIO::feof(FILE * stream) {
    BRAHMA_UNWRAPPED_FUNC(feof, int, (stream));
    return result;
}
int STDIO::ferror(FILE *stream) {
  BRAHMA_UNWRAPPED_FUNC(ferror, int, (stream));
  return result;
}
int STDIO::fflush(FILE *stream) {
  BRAHMA_UNWRAPPED_FUNC(fflush, int, (stream));
  return result;
}
int STDIO::fgetc(FILE *stream) {
  BRAHMA_UNWRAPPED_FUNC(fgetc, int, (stream));
  return result;
}
int STDIO::fgetpos(FILE *stream, fpos_t *pos) {
  BRAHMA_UNWRAPPED_FUNC(fgetpos, int, (stream, pos));
  return result;
}
int STDIO::fgetpos64(FILE *stream, fpos64_t *pos) {
  BRAHMA_UNWRAPPED_FUNC(fgetpos64, int, (stream, pos));
  return result;
}
char* STDIO::fgets(char *s, int size, FILE *stream) {
  BRAHMA_UNWRAPPED_FUNC(fgets, char*, (s, size, stream));
  return result;
}
void STDIO::flockfile(FILE *stream) {
  BRAHMA_UNWRAPPED_FUNC_VOID(flockfile, (stream));
}
int STDIO::fputc(int c, FILE *stream) {
  BRAHMA_UNWRAPPED_FUNC(fputc, int, (c, stream));
  return result;
}
int STDIO::fputs(const char *s, FILE *stream) {
  BRAHMA_UNWRAPPED_FUNC(fputs, int, (s, stream));
  return result;
}
  FILE* STDIO::freopen(const char *pathname, const char *mode, FILE *stream) {
  BRAHMA_UNWRAPPED_FUNC(freopen, FILE*, (pathname, mode, stream));
  return result;
}
FILE* STDIO::freopen64(const char *pathname, const char *mode, FILE *stream) {
  BRAHMA_UNWRAPPED_FUNC(freopen64, FILE*, (pathname, mode, stream));
  return result;
}
int STDIO::fsetpos(FILE *stream, const fpos_t *pos) {
  BRAHMA_UNWRAPPED_FUNC(fsetpos, int, (stream, pos));
  return result;
}
int STDIO::fsetpos64(FILE *stream, const fpos64_t *pos) {
  BRAHMA_UNWRAPPED_FUNC(fsetpos64, int, (stream, pos));
  return result;
}
int STDIO::ftrylockfile(FILE *stream) {
  BRAHMA_UNWRAPPED_FUNC(ftrylockfile, int, (stream));
  return result;
}
void STDIO::funlockfile(FILE *stream) {
  BRAHMA_UNWRAPPED_FUNC_VOID(funlockfile, (stream));
}
int STDIO::getc(FILE *stream) {
  BRAHMA_UNWRAPPED_FUNC(getc, int, (stream));
  return result;
}
int STDIO::getc_unlocked(FILE *stream) {
  BRAHMA_UNWRAPPED_FUNC(getc_unlocked, int, (stream));
  return result;
}
int STDIO::getw(FILE *stream) {
  BRAHMA_UNWRAPPED_FUNC(getw, int, (stream));
  return result;
}
int STDIO::pclose(FILE *stream) {
  BRAHMA_UNWRAPPED_FUNC(pclose, int, (stream));
  return result;
}
int STDIO::putw(int w, FILE *stream) {
  BRAHMA_UNWRAPPED_FUNC(putw, int, (w, stream));
  return result;
}
void STDIO::rewind(FILE *stream) {
  BRAHMA_UNWRAPPED_FUNC_VOID(rewind, (stream));
}
int STDIO::setvbuf(FILE *stream, char *buf, int mode, size_t size) {
  BRAHMA_UNWRAPPED_FUNC(setvbuf, int, (stream, buf, mode, size));
  return result;
}
int STDIO::ungetc(int c, FILE *stream) {
  BRAHMA_UNWRAPPED_FUNC(ungetc, int, (c, stream));
  return result;
}

int STDIO::fprintf(FILE *stream, const char *format, va_list args) {
  return vfprintf(stream, format, args);
}
int STDIO::printf(const char *format, va_list args) {
  return vprintf(format, args);
}
int STDIO::sprintf(char *str, const char *format, va_list args) {
  return vsprintf(str, format, args);
}
int STDIO::snprintf(char *str, size_t size, const char *format,
                    va_list args) {
  // See stdio_bypass.h: calling the plain (possibly GOTCHA-intercepted)
  // "vsnprintf" symbol here risks routing back through this library's own
  // wrapper for it depending on bind()/unbind() state, rather than always
  // reaching true libc. Resolve and call the real vsnprintf directly via
  // dlopen/dlsym instead.
  return STDIOBypass::get_instance().vsnprintf(str, size, format, args);
}
int STDIO::vfprintf(FILE *stream, const char *format, va_list args) {
  BRAHMA_UNWRAPPED_FUNC(vfprintf, int, (stream, format, args));
  return result;
}
int STDIO::vprintf(const char *format, va_list args) {
  BRAHMA_UNWRAPPED_FUNC(vprintf, int, (format, args));
  return result;
}
int STDIO::vsprintf(char *str, const char *format, va_list args) {
  BRAHMA_UNWRAPPED_FUNC(vsprintf, int, (str, format, args));
  return result;
}
int STDIO::vsnprintf(char *str, size_t size, const char *format,
                     va_list args) {
  BRAHMA_UNWRAPPED_FUNC(vsnprintf, int, (str, size, format, args));
  return result;
}
int STDIO::fscanf(FILE *stream, const char *format, va_list args) {
  return vfscanf(stream, format, args);
}
int STDIO::scanf(const char *format, va_list args) {
  return vscanf(format, args);
}
int STDIO::sscanf(const char *str, const char *format, va_list args) {
  return vsscanf(str, format, args);
}
int STDIO::vfscanf(FILE *stream, const char *format, va_list args) {
  BRAHMA_UNWRAPPED_FUNC(vfscanf, int, (stream, format, args));
  return result;
}
int STDIO::vscanf(const char *format, va_list args) {
  BRAHMA_UNWRAPPED_FUNC(vscanf, int, (format, args));
  return result;
}
int STDIO::vsscanf(const char *str, const char *format, va_list args) {
  BRAHMA_UNWRAPPED_FUNC(vsscanf, int, (str, format, args));
  return result;
}
int STDIO::__isoc23_fscanf(FILE *stream, const char *format, va_list args) {
  return __isoc23_vfscanf(stream, format, args);
}
int STDIO::__isoc23_scanf(const char *format, va_list args) {
  return __isoc23_vscanf(format, args);
}
int STDIO::__isoc23_sscanf(const char *str, const char *format,
                           va_list args) {
  return __isoc23_vsscanf(str, format, args);
}
int STDIO::__isoc23_vfscanf(FILE *stream, const char *format, va_list args) {
  BRAHMA_UNWRAPPED_FUNC(__isoc23_vfscanf, int, (stream, format, args));
  return result;
}
int STDIO::__isoc23_vscanf(const char *format, va_list args) {
  BRAHMA_UNWRAPPED_FUNC(__isoc23_vscanf, int, (format, args));
  return result;
}
int STDIO::__isoc23_vsscanf(const char *str, const char *format,
                            va_list args) {
  BRAHMA_UNWRAPPED_FUNC(__isoc23_vsscanf, int, (str, format, args));
  return result;
}
int STDIO::__isoc99_fscanf(FILE *stream, const char *format, va_list args) {
  return __isoc99_vfscanf(stream, format, args);
}
int STDIO::__isoc99_scanf(const char *format, va_list args) {
  return __isoc99_vscanf(format, args);
}
int STDIO::__isoc99_sscanf(const char *str, const char *format,
                           va_list args) {
  return __isoc99_vsscanf(str, format, args);
}
int STDIO::__isoc99_vfscanf(FILE *stream, const char *format, va_list args) {
  BRAHMA_UNWRAPPED_FUNC(__isoc99_vfscanf, int, (stream, format, args));
  return result;
}
int STDIO::__isoc99_vscanf(const char *format, va_list args) {
  BRAHMA_UNWRAPPED_FUNC(__isoc99_vscanf, int, (format, args));
  return result;
}
int STDIO::__isoc99_vsscanf(const char *str, const char *format,
                            va_list args) {
  BRAHMA_UNWRAPPED_FUNC(__isoc99_vsscanf, int, (str, format, args));
  return result;
}
size_t STDIO::__fread_chk(void *ptr, size_t ptrlen, size_t size, size_t n, FILE *stream) {
  BRAHMA_UNWRAPPED_FUNC(__fread_chk, size_t, (ptr, ptrlen, size, n, stream));
  return result;
}
size_t STDIO::__fread_unlocked_chk(void *ptr, size_t ptrlen, size_t size, size_t n, FILE *stream) {
  BRAHMA_UNWRAPPED_FUNC(__fread_unlocked_chk, size_t, (ptr, ptrlen, size, n, stream));
  return result;
}
char *STDIO::__fgets_chk(char *s, size_t size, int n, FILE *stream) {
  BRAHMA_UNWRAPPED_FUNC(__fgets_chk, char *, (s, size, n, stream));
  return result;
}
char *STDIO::__fgets_unlocked_chk(char *s, size_t size, int n, FILE *stream) {
  BRAHMA_UNWRAPPED_FUNC(__fgets_unlocked_chk, char *, (s, size, n, stream));
  return result;
}
int STDIO::puts(const char *s) {
  BRAHMA_UNWRAPPED_FUNC(puts, int, (s));
  return result;
}
int STDIO::putchar(int c) {
  BRAHMA_UNWRAPPED_FUNC(putchar, int, (c));
  return result;
}
int STDIO::putc(int c, FILE *stream) {
  BRAHMA_UNWRAPPED_FUNC(putc, int, (c, stream));
  return result;
}
int STDIO::putc_unlocked(int c, FILE *stream) {
  BRAHMA_UNWRAPPED_FUNC(putc_unlocked, int, (c, stream));
  return result;
}
int STDIO::getchar(void) {
  BRAHMA_UNWRAPPED_FUNC(getchar, int, ());
  return result;
}
int STDIO::getchar_unlocked(void) {
  BRAHMA_UNWRAPPED_FUNC(getchar_unlocked, int, ());
  return result;
}
void STDIO::perror(const char *s) {
  BRAHMA_UNWRAPPED_FUNC_VOID(perror, (s));
}
void STDIO::setbuf(FILE *stream, char *buf) {
  BRAHMA_UNWRAPPED_FUNC_VOID(setbuf, (stream, buf));
}
void STDIO::setbuffer(FILE *stream, char *buf, size_t size) {
  BRAHMA_UNWRAPPED_FUNC_VOID(setbuffer, (stream, buf, size));
}
void STDIO::setlinebuf(FILE *stream) {
  BRAHMA_UNWRAPPED_FUNC_VOID(setlinebuf, (stream));
}
ssize_t STDIO::getline(char **lineptr, size_t *n, FILE *stream) {
  BRAHMA_UNWRAPPED_FUNC(getline, ssize_t, (lineptr, n, stream));
  return result;
}
ssize_t STDIO::getdelim(char **lineptr, size_t *n, int delim, FILE *stream) {
  BRAHMA_UNWRAPPED_FUNC(getdelim, ssize_t, (lineptr, n, delim, stream));
  return result;
}
FILE *STDIO::fmemopen(void *buf, size_t size, const char *mode) {
  BRAHMA_UNWRAPPED_FUNC(fmemopen, FILE *, (buf, size, mode));
  return result;
}
FILE *STDIO::open_memstream(char **ptr, size_t *sizeloc) {
  BRAHMA_UNWRAPPED_FUNC(open_memstream, FILE *, (ptr, sizeloc));
  return result;
}
FILE *STDIO::popen(const char *command, const char *type) {
  BRAHMA_UNWRAPPED_FUNC(popen, FILE *, (command, type));
  return result;
}
char *STDIO::tmpnam(char *s) {
  BRAHMA_UNWRAPPED_FUNC(tmpnam, char *, (s));
  return result;
}


size_t brahma::STDIO::unbind() {
  num_bindings = unbindings.size();
  if (num_bindings > 0) {
    gotcha_binding_t *raw_bindings = unbindings.data();
    char unbind_name[128];
    // See STDIO::bind(): sprintf is itself an interceptable function, so it
    // can't safely be used for this internal bookkeeping.
    strcpy(unbind_name, tool_name);
    strcat(unbind_name, "_unbind");
    gotcha_wrap(raw_bindings, num_bindings, unbind_name);
    gotcha_set_priority(unbind_name, bind_priority+1);
  }
  return num_bindings;
}
}  // namespace brahma