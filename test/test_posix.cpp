//
// Created by hariharan on 8/8/22.
//
#include <assert.h>
#include <brahma/brahma.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <utime.h>
#include <signal.h>
#include <execinfo.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/uio.h>
#include <sys/statvfs.h>
#include <sys/wait.h>
#include <cstdarg>
#include <aio.h>
#include <ftw.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/vfs.h>
#include <sys/xattr.h>
#include <wchar.h>

#include <iostream>

namespace brahma {

// Logging helper for this test's own diagnostic output. printf()/puts() are
// themselves intercepted below (see STDIOTest), so an ordinary
// printf("N Captured ... call\n") call inside ANY override -- POSIXTest's
// included -- would route through STDIOTest::printf() too, inflating its
// invocation count with every unrelated log line in this file. Bypass the
// interception entirely by calling the real, unwrapped printf directly.
inline void test_log(const char *msg) {
  auto instance = STDIO::get_instance();
  auto real_printf =
      (printf_fptr)gotcha_get_wrappee(instance->printf_brahma_handle);
  real_printf("%s", msg);
}

// Same bypass, but for diagnostic prints that need real format substitution
// (this test's own summary/report lines, not a specific API's capture log).
inline void test_logf(const char *fmt, ...) {
  auto instance = STDIO::get_instance();
  auto real_vprintf =
      (vprintf_fptr)gotcha_get_wrappee(instance->vprintf_brahma_handle);
  va_list args;
  va_start(args, fmt);
  real_vprintf(fmt, args);
  va_end(args);
}

inline void test_logf_stream(FILE *stream, const char *fmt, ...) {
  auto instance = STDIO::get_instance();
  auto real_vfprintf =
      (vfprintf_fptr)gotcha_get_wrappee(instance->vfprintf_brahma_handle);
  va_list args;
  va_start(args, fmt);
  real_vfprintf(stream, fmt, args);
  va_end(args);
}

class POSIXTest : public POSIX {
 private:
  static std::shared_ptr<POSIXTest> instance;

 public:
  POSIXTest() : api_count(0) {}
  virtual ~POSIXTest() {}
  static std::shared_ptr<POSIXTest> get_instance() {
    if (instance == nullptr) {
      instance = std::make_shared<POSIXTest>();
      POSIX::set_instance(instance);
    }
    return instance;
  }
  size_t api_count;
  
  int open(const char *pathname, int flags, ...) override {
    test_log("1 Captured open call\n");
    api_count++;
    return 0;
  }

  int creat(const char *path, mode_t mode) override {
    test_log("111 Captured creat call\n");
    api_count++;
    return 0;
  }

  int creat64(const char *path, mode_t mode) override {
    test_log("2 Captured creat64 call\n");
    api_count++;
    return 0;
  }

  int open64(const char *path, int flags, ...) override {
    test_log("3 Captured open64 call\n");
    api_count++;
    return 0;
  }

  int close(int fd) override {
    test_log("4 Captured close call\n");
    api_count++;
    return 0;
  }

  ssize_t write(int fd, const void *buf, size_t count) override {
    test_log("5 Captured write call\n");
    api_count++;
    return 0;
  }

  ssize_t read(int fd, void *buf, size_t count) override {
    test_log("6 Captured read call\n");
    api_count++;
    return 0;
  }

  off_t lseek(int fd, off_t offset, int whence) override {
    test_log("7 Captured lseek call\n");
    api_count++;
    return 0;
  }

  off64_t lseek64(int fd, off64_t offset, int whence) override {
    test_log("8 Captured lseek64 call\n");
    api_count++;
    return 0;
  }

  ssize_t pread(int fd, void *buf, size_t count, off_t offset) override {
    test_log("9 Captured pread call\n");
    api_count++;
    return 0;
  }

  ssize_t pread64(int fd, void *buf, size_t count, off64_t offset) override {
    test_log("10 Captured pread64 call\n");
    api_count++;
    return 0;
  }

  ssize_t pwrite(int fd, const void *buf, size_t count,
                 off64_t offset) override {
    test_log("11 Captured pwrite call\n");
    api_count++;
    return 0;
  }

  ssize_t pwrite64(int fd, const void *buf, size_t count,
                   off64_t offset) override {
    test_log("12 Captured pwrite64 call\n");
    api_count++;
    return 0;
  }

  int fsync(int fd) override {
    test_log("13 Captured fsync call\n");
    api_count++;
    return 0;
  }

  int fdatasync(int fd) override {
    test_log("14 Captured fdatasync call\n");
    api_count++;
    return 0;
  }

  int openat(int dirfd, const char *pathname, int flags, ...) override {
    test_log("15 Captured openat call\n");
    api_count++;
    return 0;
  }


  int openat64(int dirfd, const char *pathname, int flags, ...) override {
    test_log("15 Captured openat64 call\n");
    api_count++;
    return 0;
  }
  
  char *getcwd(char *buf, size_t size) override {
    test_log("16 Captured getcwd call\n");
    api_count++;
    return 0;
  }

  // _FORTIFY_SOURCE entry points that distro builds call in place of the
  // plain functions.
  int __open_2(const char *path, int oflag) override {
    test_log("100 Captured __open_2 call\n");
    api_count++;
    return 0;
  }

  ssize_t __read_chk(int fd, void *buf, size_t nbytes, size_t buflen) override {
    test_log("101 Captured __read_chk call\n");
    api_count++;
    return 0;
  }

  char *__getcwd_chk(char *buf, size_t size, size_t buflen) override {
    test_log("102 Captured __getcwd_chk call\n");
    api_count++;
    return 0;
  }

  int mkdir(const char *pathname, mode_t mode) override {
    test_log("17 Captured mkdir call\n");
    api_count++;
    return 0;
  }

  int rmdir(const char *pathname) override {
    test_log("18 Captured rmdir call\n");
    api_count++;
    return 0;
  }

  int chdir(const char *path) override {
    test_log("19 Captured chdir call\n");
    api_count++;
    return 0;
  }

  int link(const char *oldpath, const char *newpath) override {
    test_log("20 Captured link call\n");
    api_count++;
    return 0;
  }

  int linkat(int fd1, const char *path1, int fd2, const char *path2,
             int flag) override {
    test_log("21 Captured linkat call\n");
    api_count++;
    return 0;
  }

  int unlink(const char *pathname) override {
    test_log("22 Captured unlink call\n");
    api_count++;
    return 0;
  }

  int symlink(const char *path1, const char *path2) override {
    test_log("23 Captured symlink call\n");
    api_count++;
    return 0;
  }

  int symlinkat(const char *path1, int fd, const char *path2) override {
    test_log("24 Captured symlinkat call\n");
    api_count++;
    return 0;
  }

  ssize_t readlink(const char *path, char *buf, size_t bufsize) override {
    test_log("25 Captured readlink call\n");
    api_count++;
    return 0;
  }
  ssize_t readlinkat(int fd, const char *path, char *buf,
                     size_t bufsize) override {
    test_log("26 Captured readlinkat call\n");
    api_count++;
    return 0;
  }

  int rename(const char *oldpath, const char *newpath) override {
    test_log("27 Captured rename call\n");
    api_count++;
    return 0;
  }

  int chmod(const char *path, mode_t mode) override {
    test_log("28 Captured chmod call\n");
    api_count++;
    return 0;
  }

  int chown(const char *path, uid_t owner, gid_t group) override {
    test_log("29 Captured chown call\n");
    api_count++;
    return 0;
  }

  int lchown(const char *path, uid_t owner, gid_t group) override {
    test_log("30 Captured lchown call\n");
    api_count++;
    return 0;
  }

  int utime(const char *filename, const utimbuf *buf) override {
    test_log("31 Captured utime call\n");
    api_count++;
    return 0;
  }

  DIR *opendir(const char *name) override {
    test_log("32 Captured opendir call\n");
    api_count++;
    return 0;
  }

  dirent *readdir(DIR *dir) override {
    test_log("33 Captured readdir call\n");
    api_count++;
    return 0;
  }
  dirent64 *readdir64(DIR *dir) override {
    test_log("33 Captured readdir64 call\n");
    api_count++;
    return 0;
  }
  int closedir(DIR *dir) override {
    test_log("34 Captured closedir call\n");
    api_count++;
    return 0;
  }

  void rewinddir(DIR *dir) override {
    test_log("35 Captured rewinddir call\n");
    api_count++;
  }

  int fcntl(int fd, int cmd, ...) override {
    test_log("36 Captured fcntl call\n");
    api_count++;
    return 0;
  }

  int fcntl64(int fd, int cmd, ...) override {
    test_log("36 Captured fcntl64 call\n");
    api_count++;
    return 0;
  }

  int dup(int oldfd) override {
    test_log("37 Captured dup call\n");
    api_count++;
    return 0;
  }

  int dup2(int oldfd, int newfd) override {
    test_log("38 Captured dup2 call\n");
    api_count++;
    return 0;
  }

  int dup3(int oldfd, int newfd, int flags) override {
    test_log("112 Captured dup3 call\n");
    api_count++;
    return 0;
  }

#if defined(__GLIBC__) && __GLIBC_PREREQ(2, 34)
  int close_range(unsigned int first, unsigned int last, int flags) override {
    test_log("115 Captured close_range call\n");
    api_count++;
    return 0;
  }

  void closefrom(int lowfd) override {
    test_log("116 Captured closefrom call\n");
    api_count++;
  }
#endif

  int pipe(int pipefd[2]) override {
    test_log("39 Captured pipe call\n");
    api_count++;
    return 0;
  }
  int mkfifo(const char *pathname, mode_t mode) override {
    test_log("40 Captured mkfifo call\n");
    api_count++;
    return 0;
  }

  mode_t umask(mode_t mask) override {
    test_log("41 Captured umask call\n");
    api_count++;
    return 0;
  }

  int access(const char *path, int amode) override {
    test_log("42 Captured access call\n");
    api_count++;
    return 0;
  }

  int faccessat(int fd, const char *path, int amode, int flag) override {
    test_log("43 Captured faccessat call\n");
    api_count++;
    return 0;
  }

  int remove(const char *pathname) override {
    test_log("44 Captured remove call\n");
    api_count++;
    return 0;
  }

  int truncate(const char *pathname, off_t length) override {
    test_log("45 Captured truncate call\n");
    api_count++;
    return 0;
  }
  int truncate64(const char *pathname, off64_t length) override {
    test_log("45 Captured truncate64 call\n");
    api_count++;
    return 0;
  }

  int ftruncate(int fd, off_t length) override {
    test_log("46 Captured ftruncate call\n");
    api_count++;
    return 0;
  }
  int ftruncate64(int fd, off64_t length) override {
    test_log("46 Captured ftruncate64 call\n");
    api_count++;
    return 0;
  }

  int execl(const char *pathname, const char *arg, ...) override {
    test_log("47 Captured execl call\n");
    api_count++;
    return 0;
  }

  int execlp(const char *file, const char *arg, ...) override {
    test_log("48 Captured execlp call\n");
    api_count++;
    return 0;
  }

  int execv(const char *pathname, char *const argv[]) override {
    test_log("49 Captured execv call\n");
    api_count++;
    return 0;
  }

  int execvp(const char *file, char *const argv[]) override {
    test_log("50 Captured execvp call\n");
    api_count++;
    return 0;
  }

  int execvpe(const char *file, char *const argv[],
              char *const envp[]) override {
    test_log("51 Captured execvpe call\n");
    api_count++;
    return 0;
  }

  int fork() override {
    test_log("52 Captured fork call\n");
    api_count++;
    return 0;
  }

  void exit(int status) override {
    BRAHMA_MAP_OR_FAIL(exit);
    test_logf("53 Captured exit with code %d\n", status);
    api_count++;
    __real_exit(0);
  }

  void *mmap(void *addr, size_t length, int prot, int flags, int fd,
                     off_t offset) override {
    test_log("55 Captured mmap call\n");
    api_count++;
    return 0;
  }
  
  void *mmap64(void *addr, size_t length, int prot, int flags, int fd,
                       off64_t offset) override {
    test_log("56 Captured mmap64 call\n");
    api_count++;
    return 0;
  }

  int munmap(void *addr, size_t len) override {
    test_log("57 Captured munmap call\n");
    api_count++;
    return 0;
  }

  int msync(void *addr, size_t len, int flags) override {
    test_log("58 Captured msync call\n");
    api_count++;
    return 0;
  }

  long sysconf(int name) override {
    test_log("59 Captured sysconf call\n");
    api_count++;
    return 0;
  }

  int madvise(void *addr, size_t length, int advice) override {
    test_log("60 Captured madvise call\n");
    api_count++;
    return 0;
  }

  int mprotect(void *addr, size_t length, int prot) override {
    test_log("61 Captured mprotect call\n");
    api_count++;
    return 0;
  }

  int mlock(const void *addr, size_t len) override {
    test_log("62 Captured mlock call\n");
    api_count++;
    return 0;
  }
  
  int munlock(const void *addr, size_t len) override {
    test_log("63 Captured munlock call\n");
    api_count++;
    return 0;
  }

  int mlockall(int flags) override {
    test_log("64 Captured mlockall call\n");
    api_count++;
    return 0;
  }

  int munlockall(void) override {
    test_log("65 Captured munlockall call\n");
    api_count++;
    return 0;
  }

#if defined(__GLIBC__) && __GLIBC_PREREQ(2, 32)
  // Before glibc 2.32, stat/lstat/fstat/mknod were not real exported
  // dynamic symbols -- calls were multiplexed through internal versioned
  // aliases (__xstat/__xmknod, see posix.h), so gotcha could never bind
  // the plain symbol names. 2.32 removed that indirection, so they're
  // only bindable (and therefore only tested) from that version onward.
  int stat(const char *path, struct stat *buf) override {
    test_log("66 Captured stat call\n");
    api_count++;
    return 0;
  }

  int lstat(const char *path, struct stat *buf) override {
    test_log("67 Captured lstat call\n");
    api_count++;
    return 0;
  }

  int fstat(int fd, struct stat *buf) override {
    test_log("68 Captured fstat call\n");
    api_count++;
    return 0;
  }

  int fstatat(int dirfd, const char *path, struct stat *buf,
              int flags) override {
    test_log("69 Captured fstatat call\n");
    api_count++;
    return 0;
  }

  // See GOTCHA_BINDING_MACRO_LFS64 in interceptor.h: on platforms where
  // stat/lstat/fstat/fstatat are __REDIRECT-aliased to their "64"
  // counterpart, application calls resolve directly to these instead.
  int stat64(const char *path, struct stat64 *buf) override {
    test_log("66 Captured stat64 call\n");
    api_count++;
    return 0;
  }

  int lstat64(const char *path, struct stat64 *buf) override {
    test_log("67 Captured lstat64 call\n");
    api_count++;
    return 0;
  }

  int fstat64(int fd, struct stat64 *buf) override {
    test_log("68 Captured fstat64 call\n");
    api_count++;
    return 0;
  }

  int fstatat64(int dirfd, const char *path, struct stat64 *buf,
                int flags) override {
    test_log("69 Captured fstatat64 call\n");
    api_count++;
    return 0;
  }
#endif

  int posix_fadvise(int fd, off_t offset, off_t len, int advice) override {
    test_log("70 Captured posix_fadvise call\n");
    api_count++;
    return 0;
  }

  int posix_fadvise64(int fd, off64_t offset, off64_t len,
                      int advice) override {
    test_log("70 Captured posix_fadvise64 call\n");
    api_count++;
    return 0;
  }

  int posix_fallocate(int fd, off_t offset, off_t len) override {
    test_log("71 Captured posix_fallocate call\n");
    api_count++;
    return 0;
  }

  int posix_fallocate64(int fd, off64_t offset, off64_t len) override {
    test_log("71 Captured posix_fallocate64 call\n");
    api_count++;
    return 0;
  }

  int flock(int fd, int operation) override {
    test_log("72 Captured flock call\n");
    api_count++;
    return 0;
  }

  ssize_t readv(int fd, const struct iovec *iov, int iovcnt) override {
    test_log("73 Captured readv call\n");
    api_count++;
    return 0;
  }

  ssize_t writev(int fd, const struct iovec *iov, int iovcnt) override {
    test_log("74 Captured writev call\n");
    api_count++;
    return 0;
  }

  ssize_t preadv(int fd, const struct iovec *iov, int iovcnt,
                 off_t offset) override {
    test_log("75 Captured preadv call\n");
    api_count++;
    return 0;
  }

  ssize_t preadv64(int fd, const struct iovec *iov, int iovcnt,
                   off64_t offset) override {
    test_log("75 Captured preadv64 call\n");
    api_count++;
    return 0;
  }

  ssize_t pwritev(int fd, const struct iovec *iov, int iovcnt,
                  off_t offset) override {
    test_log("76 Captured pwritev call\n");
    api_count++;
    return 0;
  }

  ssize_t pwritev64(int fd, const struct iovec *iov, int iovcnt,
                    off64_t offset) override {
    test_log("76 Captured pwritev64 call\n");
    api_count++;
    return 0;
  }

  int renameat(int olddirfd, const char *oldpath, int newdirfd,
               const char *newpath) override {
    test_log("77 Captured renameat call\n");
    api_count++;
    return 0;
  }

  int mkdirat(int dirfd, const char *pathname, mode_t mode) override {
    test_log("78 Captured mkdirat call\n");
    api_count++;
    return 0;
  }

  int unlinkat(int dirfd, const char *pathname, int flags) override {
    test_log("79 Captured unlinkat call\n");
    api_count++;
    return 0;
  }

  int fchmodat(int dirfd, const char *pathname, mode_t mode,
               int flags) override {
    test_log("80 Captured fchmodat call\n");
    api_count++;
    return 0;
  }

  int fchownat(int dirfd, const char *pathname, uid_t owner, gid_t group,
               int flags) override {
    test_log("81 Captured fchownat call\n");
    api_count++;
    return 0;
  }

  int fchmod(int fd, mode_t mode) override {
    test_log("82 Captured fchmod call\n");
    api_count++;
    return 0;
  }

  int fchown(int fd, uid_t owner, gid_t group) override {
    test_log("83 Captured fchown call\n");
    api_count++;
    return 0;
  }

  int execve(const char *pathname, char *const argv[],
             char *const envp[]) override {
    test_log("84 Captured execve call\n");
    api_count++;
    return 0;
  }

  pid_t waitpid(pid_t pid, int *wstatus, int options) override {
    test_log("85 Captured waitpid call\n");
    api_count++;
    return 0;
  }

  pid_t wait(int *wstatus) override {
    test_log("86 Captured wait call\n");
    api_count++;
    return 0;
  }

  char *realpath(const char *path, char *resolved_path) override {
    test_log("87 Captured realpath call\n");
    api_count++;
    return 0;
  }

  int dirfd(DIR *dir) override {
    test_log("88 Captured dirfd call\n");
    api_count++;
    return 0;
  }

#if defined(__GLIBC__) && __GLIBC_PREREQ(2, 32)
  // See stat/lstat/fstat/fstatat above: same pre-2.32 __xmknod indirection.
  int mknod(const char *pathname, mode_t mode, dev_t dev) override {
    test_log("89 Captured mknod call\n");
    api_count++;
    return 0;
  }
#endif

  ssize_t sendfile(int out_fd, int in_fd, off_t *offset,
                   size_t count) override {
    test_log("90 Captured sendfile call\n");
    api_count++;
    return 0;
  }

  ssize_t sendfile64(int out_fd, int in_fd, off64_t *offset,
                     size_t count) override {
    test_log("90 Captured sendfile64 call\n");
    api_count++;
    return 0;
  }

  ssize_t copy_file_range(int fd_in, off64_t *off_in, int fd_out,
                          off64_t *off_out, size_t len,
                          unsigned int flags) override {
    test_log("91 Captured copy_file_range call\n");
    api_count++;
    return 0;
  }

  int statvfs(const char *path, struct statvfs *buf) override {
    test_log("92 Captured statvfs call\n");
    api_count++;
    return 0;
  }

  int statvfs64(const char *path, struct statvfs64 *buf) override {
    test_log("92 Captured statvfs64 call\n");
    api_count++;
    return 0;
  }

  int fstatvfs(int fd, struct statvfs *buf) override {
    test_log("93 Captured fstatvfs call\n");
    api_count++;
    return 0;
  }

  int fstatvfs64(int fd, struct statvfs64 *buf) override {
    test_log("93 Captured fstatvfs64 call\n");
    api_count++;
    return 0;
  }
  ssize_t preadv2(int fd, const struct iovec *iov, int iovcnt, off_t offset, int flags) override {
    test_log("127 Captured preadv2 call\n");
    api_count++;
    return 0;
  }
  ssize_t pwritev2(int fd, const struct iovec *iov, int iovcnt, off_t offset, int flags) override {
    test_log("128 Captured pwritev2 call\n");
    api_count++;
    return 0;
  }
  ssize_t preadv64v2(int fd, const struct iovec *iov, int iovcnt, off64_t offset, int flags) override {
    test_log("129 Captured preadv64v2 call\n");
    api_count++;
    return 0;
  }
  ssize_t pwritev64v2(int fd, const struct iovec *iov, int iovcnt, off64_t offset, int flags) override {
    test_log("130 Captured pwritev64v2 call\n");
    api_count++;
    return 0;
  }
  ssize_t splice(int fd_in, off64_t *off_in, int fd_out, off64_t *off_out, size_t len, unsigned int flags) override {
    test_log("131 Captured splice call\n");
    api_count++;
    return 0;
  }
  ssize_t tee(int fd_in, int fd_out, size_t len, unsigned int flags) override {
    test_log("132 Captured tee call\n");
    api_count++;
    return 0;
  }
  ssize_t vmsplice(int fd, const struct iovec *iov, size_t nr_segs, unsigned int flags) override {
    test_log("133 Captured vmsplice call\n");
    api_count++;
    return 0;
  }
  int fallocate(int fd, int mode, off_t offset, off_t len) override {
    test_log("134 Captured fallocate call\n");
    api_count++;
    return 0;
  }
  int fallocate64(int fd, int mode, off64_t offset, off64_t len) override {
    test_log("135 Captured fallocate64 call\n");
    api_count++;
    return 0;
  }
  ssize_t readahead(int fd, off64_t offset, size_t count) override {
    test_log("136 Captured readahead call\n");
    api_count++;
    return 0;
  }
  void sync(void) override {
    test_log("137 Captured sync call\n");
    api_count++;
  }
  int syncfs(int fd) override {
    test_log("138 Captured syncfs call\n");
    api_count++;
    return 0;
  }
  int sync_file_range(int fd, off64_t offset, off64_t nbytes, unsigned int flags) override {
    test_log("139 Captured sync_file_range call\n");
    api_count++;
    return 0;
  }
  int statx(int dirfd, const char *path, int flags, unsigned int mask, struct statx *buf) override {
    test_log("140 Captured statx call\n");
    api_count++;
    return 0;
  }
  int statfs(const char *path, struct statfs *buf) override {
    test_log("141 Captured statfs call\n");
    api_count++;
    return 0;
  }
  int fstatfs(int fd, struct statfs *buf) override {
    test_log("142 Captured fstatfs call\n");
    api_count++;
    return 0;
  }
  int statfs64(const char *path, struct statfs64 *buf) override {
    test_log("143 Captured statfs64 call\n");
    api_count++;
    return 0;
  }
  int fstatfs64(int fd, struct statfs64 *buf) override {
    test_log("144 Captured fstatfs64 call\n");
    api_count++;
    return 0;
  }
#if defined(__GLIBC__) && __GLIBC_PREREQ(2, 33)
  int mknodat(int dirfd, const char *path, mode_t mode, dev_t dev) override {
    test_log("145 Captured mknodat call\n");
    api_count++;
    return 0;
  }
#endif
#if defined(__GLIBC__) && !__GLIBC_PREREQ(2, 33)
  int __xmknodat(int ver, int dirfd, const char *path, mode_t mode, dev_t *dev) override {
    test_log("146 Captured __xmknodat call\n");
    api_count++;
    return 0;
  }
#endif
  int euidaccess(const char *path, int mode) override {
    test_log("147 Captured euidaccess call\n");
    api_count++;
    return 0;
  }
  int eaccess(const char *path, int mode) override {
    test_log("148 Captured eaccess call\n");
    api_count++;
    return 0;
  }
  int renameat2(int olddirfd, const char *oldpath, int newdirfd, const char *newpath, unsigned int flags) override {
    test_log("149 Captured renameat2 call\n");
    api_count++;
    return 0;
  }
  int mkfifoat(int dirfd, const char *path, mode_t mode) override {
    test_log("150 Captured mkfifoat call\n");
    api_count++;
    return 0;
  }
  int lchmod(const char *path, mode_t mode) override {
    test_log("151 Captured lchmod call\n");
    api_count++;
    return 0;
  }
  int utimes(const char *path, const struct timeval times[2]) override {
    test_log("152 Captured utimes call\n");
    api_count++;
    return 0;
  }
  int futimes(int fd, const struct timeval tv[2]) override {
    test_log("153 Captured futimes call\n");
    api_count++;
    return 0;
  }
  int lutimes(const char *path, const struct timeval tv[2]) override {
    test_log("154 Captured lutimes call\n");
    api_count++;
    return 0;
  }
  int futimesat(int dirfd, const char *path, const struct timeval times[2]) override {
    test_log("155 Captured futimesat call\n");
    api_count++;
    return 0;
  }
  int utimensat(int dirfd, const char *path, const struct timespec times[2], int flags) override {
    test_log("156 Captured utimensat call\n");
    api_count++;
    return 0;
  }
  int futimens(int fd, const struct timespec times[2]) override {
    test_log("157 Captured futimens call\n");
    api_count++;
    return 0;
  }
  int fchdir(int fd) override {
    test_log("158 Captured fchdir call\n");
    api_count++;
    return 0;
  }
  DIR *fdopendir(int fd) override {
    test_log("159 Captured fdopendir call\n");
    api_count++;
    return nullptr;
  }
  int readdir_r(DIR *dir, struct dirent *entry, struct dirent **res) override {
    test_log("160 Captured readdir_r call\n");
    api_count++;
    return 0;
  }
  int readdir64_r(DIR *dir, struct dirent64 *entry, struct dirent64 **res) override {
    test_log("161 Captured readdir64_r call\n");
    api_count++;
    return 0;
  }
  void seekdir(DIR *dir, long loc) override {
    test_log("162 Captured seekdir call\n");
    api_count++;
  }
  long telldir(DIR *dir) override {
    test_log("163 Captured telldir call\n");
    api_count++;
    return 0;
  }
  int scandir(const char *dir, struct dirent ***namelist, int (*filter)(const struct dirent *), int (*compar)(const struct dirent **, const struct dirent **)) override {
    test_log("164 Captured scandir call\n");
    api_count++;
    return 0;
  }
  int scandir64(const char *dir, struct dirent64 ***namelist, int (*filter)(const struct dirent64 *), int (*compar)(const struct dirent64 **, const struct dirent64 **)) override {
    test_log("165 Captured scandir64 call\n");
    api_count++;
    return 0;
  }
  int scandirat(int dirfd, const char *dir, struct dirent ***namelist, int (*filter)(const struct dirent *), int (*compar)(const struct dirent **, const struct dirent **)) override {
    test_log("166 Captured scandirat call\n");
    api_count++;
    return 0;
  }
  int scandirat64(int dirfd, const char *dir, struct dirent64 ***namelist, int (*filter)(const struct dirent64 *), int (*compar)(const struct dirent64 **, const struct dirent64 **)) override {
    test_log("240 Captured scandirat64 call\n");
    api_count++;
    return 0;
  }
  int nftw(const char *path, int (*func)(const char *, const struct stat *, int, struct FTW *), int nopenfd, int flags) override {
    test_log("167 Captured nftw call\n");
    api_count++;
    return 0;
  }
  int nftw64(const char *path, int (*func)(const char *, const struct stat64 *, int, struct FTW *), int nopenfd, int flags) override {
    test_log("168 Captured nftw64 call\n");
    api_count++;
    return 0;
  }
#if defined(__GLIBC__) && __GLIBC_PREREQ(2, 30)
  ssize_t getdents64(int fd, void *buf, size_t count) override {
    test_log("169 Captured getdents64 call\n");
    api_count++;
    return 0;
  }
#endif
  void *mremap(void *old_addr, size_t old_size, size_t new_size, int flags, void *new_addr) override {
    test_log("170 Captured mremap call\n");
    api_count++;
    return nullptr;
  }
  int ioctl(int fd, unsigned long request, void *arg) override {
    test_log("171 Captured ioctl call\n");
    api_count++;
    return 0;
  }
  int lockf(int fd, int cmd, off_t len) override {
    test_log("172 Captured lockf call\n");
    api_count++;
    return 0;
  }
  int lockf64(int fd, int cmd, off64_t len) override {
    test_log("173 Captured lockf64 call\n");
    api_count++;
    return 0;
  }
  int pipe2(int pipefd[2], int flags) override {
    test_log("174 Captured pipe2 call\n");
    api_count++;
    return 0;
  }
  int memfd_create(const char *name, unsigned int flags) override {
    test_log("175 Captured memfd_create call\n");
    api_count++;
    return 0;
  }
  int mkstemp(char *tmpl) override {
    test_log("176 Captured mkstemp call\n");
    api_count++;
    return 0;
  }
  int mkstemp64(char *tmpl) override {
    test_log("177 Captured mkstemp64 call\n");
    api_count++;
    return 0;
  }
  int mkostemp(char *tmpl, int flags) override {
    test_log("178 Captured mkostemp call\n");
    api_count++;
    return 0;
  }
  int mkostemp64(char *tmpl, int flags) override {
    test_log("179 Captured mkostemp64 call\n");
    api_count++;
    return 0;
  }
  int mkstemps(char *tmpl, int suffixlen) override {
    test_log("180 Captured mkstemps call\n");
    api_count++;
    return 0;
  }
  int mkstemps64(char *tmpl, int suffixlen) override {
    test_log("181 Captured mkstemps64 call\n");
    api_count++;
    return 0;
  }
  int mkostemps(char *tmpl, int suffixlen, int flags) override {
    test_log("182 Captured mkostemps call\n");
    api_count++;
    return 0;
  }
  int mkostemps64(char *tmpl, int suffixlen, int flags) override {
    test_log("183 Captured mkostemps64 call\n");
    api_count++;
    return 0;
  }
  char *mkdtemp(char *tmpl) override {
    test_log("184 Captured mkdtemp call\n");
    api_count++;
    return nullptr;
  }
  ssize_t getxattr(const char *path, const char *name, void *value, size_t size) override {
    test_log("185 Captured getxattr call\n");
    api_count++;
    return 0;
  }
  ssize_t lgetxattr(const char *path, const char *name, void *value, size_t size) override {
    test_log("186 Captured lgetxattr call\n");
    api_count++;
    return 0;
  }
  ssize_t fgetxattr(int fd, const char *name, void *value, size_t size) override {
    test_log("187 Captured fgetxattr call\n");
    api_count++;
    return 0;
  }
  int setxattr(const char *path, const char *name, const void *value, size_t size, int flags) override {
    test_log("188 Captured setxattr call\n");
    api_count++;
    return 0;
  }
  int lsetxattr(const char *path, const char *name, const void *value, size_t size, int flags) override {
    test_log("189 Captured lsetxattr call\n");
    api_count++;
    return 0;
  }
  int fsetxattr(int fd, const char *name, const void *value, size_t size, int flags) override {
    test_log("190 Captured fsetxattr call\n");
    api_count++;
    return 0;
  }
  ssize_t listxattr(const char *path, char *list, size_t size) override {
    test_log("191 Captured listxattr call\n");
    api_count++;
    return 0;
  }
  ssize_t llistxattr(const char *path, char *list, size_t size) override {
    test_log("192 Captured llistxattr call\n");
    api_count++;
    return 0;
  }
  ssize_t flistxattr(int fd, char *list, size_t size) override {
    test_log("193 Captured flistxattr call\n");
    api_count++;
    return 0;
  }
  int removexattr(const char *path, const char *name) override {
    test_log("194 Captured removexattr call\n");
    api_count++;
    return 0;
  }
  int lremovexattr(const char *path, const char *name) override {
    test_log("195 Captured lremovexattr call\n");
    api_count++;
    return 0;
  }
  int fremovexattr(int fd, const char *name) override {
    test_log("196 Captured fremovexattr call\n");
    api_count++;
    return 0;
  }
  int aio_read(struct aiocb *cb) override {
    test_log("197 Captured aio_read call\n");
    api_count++;
    return 0;
  }
  int aio_write(struct aiocb *cb) override {
    test_log("198 Captured aio_write call\n");
    api_count++;
    return 0;
  }
  int aio_error(const struct aiocb *cb) override {
    test_log("199 Captured aio_error call\n");
    api_count++;
    return 0;
  }
  ssize_t aio_return(struct aiocb *cb) override {
    test_log("200 Captured aio_return call\n");
    api_count++;
    return 0;
  }
  int aio_fsync(int op, struct aiocb *cb) override {
    test_log("201 Captured aio_fsync call\n");
    api_count++;
    return 0;
  }
  int aio_cancel(int fd, struct aiocb *cb) override {
    test_log("202 Captured aio_cancel call\n");
    api_count++;
    return 0;
  }
  int aio_suspend(const struct aiocb *const list[], int nent, const struct timespec *timeout) override {
    test_log("203 Captured aio_suspend call\n");
    api_count++;
    return 0;
  }
  int lio_listio(int mode, struct aiocb *const list[], int nent, struct sigevent *sig) override {
    test_log("204 Captured lio_listio call\n");
    api_count++;
    return 0;
  }
  int aio_read64(struct aiocb64 *cb) override {
    test_log("205 Captured aio_read64 call\n");
    api_count++;
    return 0;
  }
  int aio_write64(struct aiocb64 *cb) override {
    test_log("206 Captured aio_write64 call\n");
    api_count++;
    return 0;
  }
  int aio_error64(const struct aiocb64 *cb) override {
    test_log("207 Captured aio_error64 call\n");
    api_count++;
    return 0;
  }
  ssize_t aio_return64(struct aiocb64 *cb) override {
    test_log("208 Captured aio_return64 call\n");
    api_count++;
    return 0;
  }
  int aio_fsync64(int op, struct aiocb64 *cb) override {
    test_log("209 Captured aio_fsync64 call\n");
    api_count++;
    return 0;
  }
  int aio_cancel64(int fd, struct aiocb64 *cb) override {
    test_log("210 Captured aio_cancel64 call\n");
    api_count++;
    return 0;
  }
  int aio_suspend64(const struct aiocb64 *const list[], int nent, const struct timespec *timeout) override {
    test_log("211 Captured aio_suspend64 call\n");
    api_count++;
    return 0;
  }
  int lio_listio64(int mode, struct aiocb64 *const list[], int nent, struct sigevent *sig) override {
    test_log("212 Captured lio_listio64 call\n");
    api_count++;
    return 0;
  }
};
class STDIOTest : public STDIO {
 private:
  static std::shared_ptr<STDIOTest> instance;

 public:
  STDIOTest() : api_count(0) {}

  ~STDIOTest() {}
  static std::shared_ptr<STDIOTest> get_instance() {
    if (instance == nullptr) {
      instance = std::make_shared<STDIOTest>();
      STDIOTest::set_instance(instance);
    }
    return instance;
  }
  size_t api_count;

  FILE *fopen(const char *path, const char *mode) override {
    test_log("1 Captured fopen call\n");
    api_count++;
    return 0;
  }
  FILE *fopen64(const char *path, const char *mode) override {
    test_log("2 Captured fopen64 call\n");
    api_count++;
    return 0;
  }
  int fclose(FILE *fp) override {
    test_log("3 Captured fclose call\n");
    api_count++;
    return 0;
  }
  size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream) override {
    test_log("4 Captured fread call\n");
    api_count++;
    return 0;
  }
  size_t fwrite(const void *ptr, size_t size, size_t nmemb,
                FILE *stream) override {
    test_log("5 Captured fwrite call\n");
    api_count++;
    return 0;
  }
  long ftell(FILE *fp) override {
    test_log("6 Captured ftell call\n");
    api_count++;
    return 0;
  }
  int fseek(FILE *stream, long offset, int whence) override {
    test_log("7 Captured fseek call\n");
    api_count++;
    return 0;
  }
  FILE *fdopen(int fd, const char *mode) override {
    test_log("8 Captured fdopen call\n");
    api_count++;
    return 0;
  }
  int fileno(FILE *stream) override {
    test_log("9 Captured fileno call\n");
    api_count++;
    return 0;
  }
  FILE *tmpfile(void) override {
    test_log("10 Captured tmpfile call\n");
    api_count++;
    return 0;
  }
  FILE *tmpfile64(void) override {
    test_log("10 Captured tmpfile call\n");
    api_count++;
    return 0;
  }
  int fseeko(FILE *stream, off_t offset, int whence) override {
    test_log("11 Captured fseeko call\n");
    api_count++;
    return 0;
  }

  int fseeko64(FILE *stream, off64_t offset, int whence) override {
    test_log("11 Captured fseeko64 call\n");
    api_count++;
    return 0;
  }
  off_t ftello(FILE *stream) override {
    test_log("12 Captured ftello call\n");
    api_count++;
    return 0;
  }
  off64_t ftello64(FILE *stream) override {
    test_log("12 Captured ftello64 call\n");
    api_count++;
    return 0;
  }

  void clearerr(FILE *) override {
    test_log("13 Captured clearerr call\n");
    api_count++;
  }
  
  int feof(FILE *) override {
    test_log("14 Captured feof call\n");
    api_count++;
    return 0;
  }
  
  int ferror(FILE *) override {
    test_log("15 Captured ferror call\n");
    api_count++;
    return 0;
  }
  
  int fflush(FILE *) override {
    test_log("16 Captured fflush call\n");
    api_count++;
    return 0;
  }
  
  int fgetc(FILE *) override {
    test_log("17 Captured fgetc call\n");
    api_count++;
    return 0;
  }
  
  int fgetpos(FILE *, fpos_t *) override {
    test_log("18 Captured fgetpos call\n");
    api_count++;
    return 0;
  }
  int fgetpos64(FILE *, fpos64_t *) override {
    test_log("18 Captured fgetpos64 call\n");
    api_count++;
    return 0;
  }
  
  size_t __fread_chk(void *ptr, size_t ptrlen, size_t size, size_t n,
                     FILE *stream) override {
    test_log("103 Captured __fread_chk call\n");
    api_count++;
    return 0;
  }
  char* __fgets_chk(char *s, size_t size, int n, FILE *stream) override {
    test_log("104 Captured __fgets_chk call\n");
    api_count++;
    return 0;
  }
  int __fprintf_chk(FILE *stream, int flag, const char *format,
                    va_list args) override {
    test_log("113 Captured __fprintf_chk call\n");
    api_count++;
    return 0;
  }
  int __vfprintf_chk(FILE *stream, int flag, const char *format,
                     va_list args) override {
    test_log("114 Captured __vfprintf_chk call\n");
    api_count++;
    return 0;
  }
  int dprintf(int fd, const char *format, va_list args) override {
    test_log("117 Captured dprintf call\n");
    api_count++;
    return 0;
  }
  int vdprintf(int fd, const char *format, va_list args) override {
    test_log("118 Captured vdprintf call\n");
    api_count++;
    return 0;
  }
  int __dprintf_chk(int fd, int flag, const char *format, va_list args) override {
    test_log("119 Captured __dprintf_chk call\n");
    api_count++;
    return 0;
  }
  int __vdprintf_chk(int fd, int flag, const char *format, va_list args) override {
    test_log("120 Captured __vdprintf_chk call\n");
    api_count++;
    return 0;
  }
  size_t fwrite_unlocked(const void *ptr, size_t size, size_t n, FILE *stream) override {
    test_log("121 Captured fwrite_unlocked call\n");
    api_count++;
    return 0;
  }
  size_t fread_unlocked(void *ptr, size_t size, size_t n, FILE *stream) override {
    test_log("122 Captured fread_unlocked call\n");
    api_count++;
    return 0;
  }
  int fputs_unlocked(const char *s, FILE *stream) override {
    test_log("123 Captured fputs_unlocked call\n");
    api_count++;
    return 0;
  }
  char *fgets_unlocked(char *s, int n, FILE *stream) override {
    test_log("124 Captured fgets_unlocked call\n");
    api_count++;
    return nullptr;
  }
  int fputc_unlocked(int c, FILE *stream) override {
    test_log("125 Captured fputc_unlocked call\n");
    api_count++;
    return 0;
  }
  int fgetc_unlocked(FILE *stream) override {
    test_log("126 Captured fgetc_unlocked call\n");
    api_count++;
    return 0;
  }
  char* fgets(char *, int, FILE *) override {
    test_log("19 Captured fgets call\n");
    api_count++;
    return nullptr;
  }
  
  void flockfile(FILE *) override {
    test_log("20 Captured flockfile call\n");
    api_count++;
  }
  
  int fputc(int, FILE *) override {
    test_log("21 Captured fputc call\n");
    api_count++;
    return 0;
  }
  
  int fputs(const char *, FILE *) override {
    test_log("22 Captured fputs call\n");
    api_count++;
    return 0;
  }
  FILE *freopen(const char *, const char *, FILE *) override {
    test_log("23 Captured freopen call\n");
    api_count++;
    return 0;
  }
  FILE *freopen64(const char *, const char *, FILE *) override {
    test_log("23 Captured freopen64 call\n");
    api_count++;
    return 0;
  }
  int fsetpos(FILE *, const fpos_t *) override {
    test_log("24 Captured fsetpos call\n");
    api_count++;
    return 0;
  }
  int fsetpos64(FILE *, const fpos64_t *) override {
    test_log("24 Captured fsetpos64 call\n");
    api_count++;
    return 0;
  }
  
  int ftrylockfile(FILE *) override {
    test_log("25 Captured ftrylockfile call\n");
    api_count++;
    return 0;
  }
  
  void funlockfile(FILE *) override {
    test_log("26 Captured funlockfile call\n");
    api_count++;
  }
  
  int getc(FILE *) override {
    test_log("27 Captured getc call\n");
    api_count++;
    return 0;
  }
  
  int getc_unlocked(FILE *) override {
    test_log("28 Captured getc_unlocked call\n");
    api_count++;
    return 0;
  }
  
  int getw(FILE *) override {
    test_log("29 Captured getw call\n");
    api_count++;
    return 0;
  }
  
  int pclose(FILE *) override {
    test_log("30 Captured pclose call\n");
    api_count++;
    return 0;
  }
  
  int putw(int, FILE *) override {
    test_log("31 Captured putw call\n");
    api_count++;
    return 0;
  }
  
  void rewind(FILE *) override {
    test_log("32 Captured rewind call\n");
    api_count++;
  }
  
  int setvbuf(FILE *, char *, int, size_t) override {
    test_log("33 Captured setvbuf call\n");
    api_count++;
    return 0;
  }
  
  int ungetc(int, FILE *) override {
    test_log("34 Captured ungetc call\n");
    api_count++;
    return 0;
  }

  int fprintf(FILE *stream, const char *format, va_list args) override {
    test_log("35 Captured fprintf call\n");
    api_count++;
    return 0;
  }
  int printf(const char *format, va_list args) override {
    // Logging via printf()/puts() here would recurse back through this same
    // override (printf<->puts both intercepted), so log via the real
    // unwrapped printf, same pattern as POSIXTest::exit's __real_exit below.
    BRAHMA_MAP_OR_FAIL(printf);
    __real_printf("36 Captured printf call\n");
    api_count++;
    return 0;
  }
  int sprintf(char *str, const char *format, va_list args) override {
    test_log("37 Captured sprintf call\n");
    api_count++;
    return 0;
  }
  int snprintf(char *str, size_t size, const char *format,
               va_list args) override {
    test_log("38 Captured snprintf call\n");
    api_count++;
    return 0;
  }
  int vfprintf(FILE *stream, const char *format, va_list args) override {
    test_log("39 Captured vfprintf call\n");
    api_count++;
    return 0;
  }
  int vprintf(const char *format, va_list args) override {
    test_log("40 Captured vprintf call\n");
    api_count++;
    return 0;
  }
  int vsprintf(char *str, const char *format, va_list args) override {
    test_log("41 Captured vsprintf call\n");
    api_count++;
    return 0;
  }
  int vsnprintf(char *str, size_t size, const char *format,
                va_list args) override {
    test_log("42 Captured vsnprintf call\n");
    api_count++;
    return 0;
  }
  int fscanf(FILE *stream, const char *format, va_list args) override {
    test_log("43 Captured fscanf call\n");
    api_count++;
    return 0;
  }
  int scanf(const char *format, va_list args) override {
    test_log("44 Captured scanf call\n");
    api_count++;
    return 0;
  }
  int sscanf(const char *str, const char *format, va_list args) override {
    test_log("45 Captured sscanf call\n");
    api_count++;
    return 0;
  }
  int vfscanf(FILE *stream, const char *format, va_list args) override {
    test_log("46 Captured vfscanf call\n");
    api_count++;
    return 0;
  }
  int vscanf(const char *format, va_list args) override {
    test_log("47 Captured vscanf call\n");
    api_count++;
    return 0;
  }
  int vsscanf(const char *str, const char *format, va_list args) override {
    test_log("48 Captured vsscanf call\n");
    api_count++;
    return 0;
  }
  // Before glibc 2.38 the header sends fscanf and friends here in ISO C99 mode.
  int __isoc99_fscanf(FILE *stream, const char *format,
                      va_list args) override {
    test_log("105 Captured __isoc99_fscanf call\n");
    api_count++;
    return 0;
  }
  int __isoc99_scanf(const char *format, va_list args) override {
    test_log("106 Captured __isoc99_scanf call\n");
    api_count++;
    return 0;
  }
  int __isoc99_sscanf(const char *str, const char *format,
                      va_list args) override {
    test_log("107 Captured __isoc99_sscanf call\n");
    api_count++;
    return 0;
  }
  int __isoc99_vfscanf(FILE *stream, const char *format,
                       va_list args) override {
    test_log("108 Captured __isoc99_vfscanf call\n");
    api_count++;
    return 0;
  }
  int __isoc99_vscanf(const char *format, va_list args) override {
    test_log("109 Captured __isoc99_vscanf call\n");
    api_count++;
    return 0;
  }
  int __isoc99_vsscanf(const char *str, const char *format,
                       va_list args) override {
    test_log("110 Captured __isoc99_vsscanf call\n");
    api_count++;
    return 0;
  }
#if defined(__GLIBC__) && __GLIBC_PREREQ(2, 38)
  int __isoc23_fscanf(FILE *stream, const char *format,
                      va_list args) override {
    test_log("43 Captured __isoc23_fscanf call\n");
    api_count++;
    return 0;
  }
  int __isoc23_scanf(const char *format, va_list args) override {
    test_log("44 Captured __isoc23_scanf call\n");
    api_count++;
    return 0;
  }
  int __isoc23_sscanf(const char *str, const char *format,
                      va_list args) override {
    test_log("45 Captured __isoc23_sscanf call\n");
    api_count++;
    return 0;
  }
  int __isoc23_vfscanf(FILE *stream, const char *format,
                       va_list args) override {
    test_log("46 Captured __isoc23_vfscanf call\n");
    api_count++;
    return 0;
  }
  int __isoc23_vscanf(const char *format, va_list args) override {
    test_log("47 Captured __isoc23_vscanf call\n");
    api_count++;
    return 0;
  }
  int __isoc23_vsscanf(const char *str, const char *format,
                       va_list args) override {
    test_log("48 Captured __isoc23_vsscanf call\n");
    api_count++;
    return 0;
  }
#endif
  int puts(const char *s) override {
    // See printf() override above: log via the real unwrapped printf to
    // avoid recursing back through the puts()/printf() interceptors.
    BRAHMA_MAP_OR_FAIL(printf);
    __real_printf("49 Captured puts call\n");
    api_count++;
    return 0;
  }
  int putchar(int c) override {
    test_log("50 Captured putchar call\n");
    api_count++;
    return 0;
  }
  int putc(int c, FILE *stream) override {
    test_log("51 Captured putc call\n");
    api_count++;
    return 0;
  }
  int putc_unlocked(int c, FILE *stream) override {
    test_log("52 Captured putc_unlocked call\n");
    api_count++;
    return 0;
  }
  int getchar(void) override {
    test_log("53 Captured getchar call\n");
    api_count++;
    return 0;
  }
  int getchar_unlocked(void) override {
    test_log("54 Captured getchar_unlocked call\n");
    api_count++;
    return 0;
  }
  void perror(const char *s) override {
    test_log("55 Captured perror call\n");
    api_count++;
  }
  void setbuf(FILE *stream, char *buf) override {
    test_log("56 Captured setbuf call\n");
    api_count++;
  }
  void setbuffer(FILE *stream, char *buf, size_t size) override {
    test_log("57 Captured setbuffer call\n");
    api_count++;
  }
  void setlinebuf(FILE *stream) override {
    test_log("58 Captured setlinebuf call\n");
    api_count++;
  }
  ssize_t getline(char **lineptr, size_t *n, FILE *stream) override {
    test_log("59 Captured getline call\n");
    api_count++;
    return 0;
  }
  ssize_t getdelim(char **lineptr, size_t *n, int delim,
                   FILE *stream) override {
    test_log("60 Captured getdelim call\n");
    api_count++;
    return 0;
  }
  FILE *fmemopen(void *buf, size_t size, const char *mode) override {
    test_log("61 Captured fmemopen call\n");
    api_count++;
    return 0;
  }
  FILE *open_memstream(char **ptr, size_t *sizeloc) override {
    test_log("62 Captured open_memstream call\n");
    api_count++;
    return 0;
  }
  FILE *popen(const char *command, const char *type) override {
    test_log("63 Captured popen call\n");
    api_count++;
    return 0;
  }
  char *tmpnam(char *s) override {
    test_log("64 Captured tmpnam call\n");
    api_count++;
    return 0;
  }
  int fcloseall(void) override {
    test_log("213 Captured fcloseall call\n");
    api_count++;
    return 0;
  }
  FILE *fopencookie(void *cookie, const char *mode, cookie_io_functions_t funcs) override {
    test_log("214 Captured fopencookie call\n");
    api_count++;
    return nullptr;
  }
  FILE *open_wmemstream(wchar_t **ptr, size_t *sizeloc) override {
    test_log("215 Captured open_wmemstream call\n");
    api_count++;
    return nullptr;
  }
  int fflush_unlocked(FILE *stream) override {
    test_log("216 Captured fflush_unlocked call\n");
    api_count++;
    return 0;
  }
  int feof_unlocked(FILE *stream) override {
    test_log("217 Captured feof_unlocked call\n");
    api_count++;
    return 0;
  }
  int ferror_unlocked(FILE *stream) override {
    test_log("218 Captured ferror_unlocked call\n");
    api_count++;
    return 0;
  }
  int fileno_unlocked(FILE *stream) override {
    test_log("219 Captured fileno_unlocked call\n");
    api_count++;
    return 0;
  }
  void clearerr_unlocked(FILE *stream) override {
    test_log("220 Captured clearerr_unlocked call\n");
    api_count++;
  }
  int _IO_getc(FILE *stream) override {
    test_log("221 Captured _IO_getc call\n");
    api_count++;
    return 0;
  }
  int _IO_putc(int c, FILE *stream) override {
    test_log("222 Captured _IO_putc call\n");
    api_count++;
    return 0;
  }
  wint_t fgetwc(FILE *stream) override {
    test_log("223 Captured fgetwc call\n");
    api_count++;
    return 0;
  }
  wint_t getwc(FILE *stream) override {
    test_log("224 Captured getwc call\n");
    api_count++;
    return 0;
  }
  wint_t fputwc(wchar_t wc, FILE *stream) override {
    test_log("225 Captured fputwc call\n");
    api_count++;
    return 0;
  }
  wint_t putwc(wchar_t wc, FILE *stream) override {
    test_log("226 Captured putwc call\n");
    api_count++;
    return 0;
  }
  wint_t ungetwc(wint_t wc, FILE *stream) override {
    test_log("227 Captured ungetwc call\n");
    api_count++;
    return 0;
  }
  wchar_t *fgetws(wchar_t *ws, int n, FILE *stream) override {
    test_log("228 Captured fgetws call\n");
    api_count++;
    return nullptr;
  }
  int fputws(const wchar_t *ws, FILE *stream) override {
    test_log("229 Captured fputws call\n");
    api_count++;
    return 0;
  }
  int vfwprintf(FILE *stream, const wchar_t *format, va_list args) override {
    test_log("230 Captured vfwprintf call\n");
    api_count++;
    return 0;
  }
  int fwprintf(FILE *stream, const wchar_t *format, va_list args) override {
    test_log("231 Captured fwprintf call\n");
    api_count++;
    return 0;
  }
  int __vfwprintf_chk(FILE *stream, int flag, const wchar_t *format, va_list args) override {
    test_log("232 Captured __vfwprintf_chk call\n");
    api_count++;
    return 0;
  }
  int __fwprintf_chk(FILE *stream, int flag, const wchar_t *format, va_list args) override {
    test_log("233 Captured __fwprintf_chk call\n");
    api_count++;
    return 0;
  }
  int vfwscanf(FILE *stream, const wchar_t *format, va_list args) override {
    test_log("234 Captured vfwscanf call\n");
    api_count++;
    return 0;
  }
  int fwscanf(FILE *stream, const wchar_t *format, va_list args) override {
    test_log("235 Captured fwscanf call\n");
    api_count++;
    return 0;
  }
  int __isoc99_vfwscanf(FILE *stream, const wchar_t *format, va_list args) override {
    test_log("236 Captured __isoc99_vfwscanf call\n");
    api_count++;
    return 0;
  }
  int __isoc99_fwscanf(FILE *stream, const wchar_t *format, va_list args) override {
    test_log("237 Captured __isoc99_fwscanf call\n");
    api_count++;
    return 0;
  }
#if defined(__GLIBC__) && __GLIBC_PREREQ(2, 38)
  int __isoc23_vfwscanf(FILE *stream, const wchar_t *format, va_list args) override {
    test_log("238 Captured __isoc23_vfwscanf call\n");
    api_count++;
    return 0;
  }
#endif
#if defined(__GLIBC__) && __GLIBC_PREREQ(2, 38)
  int __isoc23_fwscanf(FILE *stream, const wchar_t *format, va_list args) override {
    test_log("239 Captured __isoc23_fwscanf call\n");
    api_count++;
    return 0;
  }
#endif
};

std::shared_ptr<POSIXTest> POSIXTest::instance = nullptr;
std::shared_ptr<STDIOTest> STDIOTest::instance = nullptr;

}  // namespace brahma

void __attribute__((constructor)) test_init() {
  auto posix = brahma::POSIXTest::get_instance();
  posix->bind<brahma::POSIXTest>("tool", 1);
  auto stdio = brahma::STDIOTest::get_instance();
  stdio->bind<brahma::STDIOTest>("tool", 1);
}
void __attribute__((destructor)) test_finalize() {
  // printf is still actively intercepted at this point (stdio->unbind()
  // hasn't run yet below), so this and the next diagnostic line must use
  // the bypass logger too, or they'd be swallowed by STDIOTest::printf()
  // and inflate its count. Everything after stdio->unbind() runs against
  // the real functions again and can use plain printf() safely.
  brahma::test_logf("finalizing test\n");
  auto posix = brahma::POSIXTest::get_instance();
  size_t unbindings = posix->unbind();
  auto stdio = brahma::STDIOTest::get_instance();
  brahma::test_logf("STDIO num_bindings: %zu, api_count: %zu\n",
                    stdio->num_bindings, stdio->api_count);
  size_t stdio_unbindings = stdio->unbind();
  printf("POSIX num_bindings: %zu, api_count: %zu\n", posix->num_bindings, posix->api_count);
  printf("POSIX unbindings: %zu\n", unbindings);
  fflush(stdout);
  assert(posix->num_bindings == posix->api_count);
  printf("STDIO unbindings: %zu\n", stdio_unbindings);
  assert(stdio->num_bindings == stdio->api_count);
  // Make more calls after unbind to ensure that api count isn't getting updated
  FILE *fi = fopen("test.txt", "w");
  const int size = 15;
  char buf[size] = "Hello, World!\0";
  fwrite(buf, 1, size, fi);
  fclose(fi);
  fi = fopen("test.txt", "r");
  char buf2[size];
  fread(buf2, 1, size, fi);
  printf("buf: %s\n", buf2);
  fclose(fi);
  assert(stdio->num_bindings == stdio->api_count);
}

void call_va_stdio_apis(const char *fmt, ...) {
  char buf[64];
  va_list a;
  va_start(a, fmt);
  vfprintf(stdout, fmt, a);
  va_end(a);
  va_start(a, fmt);
  __vfprintf_chk(stdout, 1, fmt, a);
  va_end(a);
  va_start(a, fmt);
  vdprintf(1, fmt, a);
  va_end(a);
  va_start(a, fmt);
  __vdprintf_chk(1, 1, fmt, a);
  va_end(a);
  va_start(a, fmt);
  vprintf(fmt, a);
  va_end(a);
  va_start(a, fmt);
  vsprintf(buf, fmt, a);
  va_end(a);
  va_start(a, fmt);
  vsnprintf(buf, sizeof(buf), fmt, a);
  va_end(a);
  va_start(a, fmt);
  vfscanf(stdin, fmt, a);
  va_end(a);
  va_start(a, fmt);
  vscanf(fmt, a);
  va_end(a);
  va_start(a, fmt);
  vsscanf(fmt, fmt, a);
  va_end(a);
  va_start(a, fmt);
  __isoc99_vfscanf(stdin, fmt, a);
  va_end(a);
  va_start(a, fmt);
  __isoc99_vscanf(fmt, a);
  va_end(a);
  va_start(a, fmt);
  __isoc99_vsscanf(fmt, fmt, a);
  va_end(a);
#if defined(__GLIBC__) && __GLIBC_PREREQ(2, 38)
  va_start(a, fmt);
  __isoc23_vfscanf(stdin, fmt, a);
  va_end(a);
  va_start(a, fmt);
  __isoc23_vscanf(fmt, a);
  va_end(a);
  va_start(a, fmt);
  __isoc23_vsscanf(fmt, fmt, a);
  va_end(a);
#endif
  va_start(a, fmt);
  vfwprintf(stdout, L"", a);
  va_end(a);
  va_start(a, fmt);
  __vfwprintf_chk(stdout, 1, L"", a);
  va_end(a);
  va_start(a, fmt);
  vfwscanf(stdin, L"", a);
  va_end(a);
  va_start(a, fmt);
  __isoc99_vfwscanf(stdin, L"", a);
  va_end(a);
#if defined(__GLIBC__) && __GLIBC_PREREQ(2, 38)
  va_start(a, fmt);
  __isoc23_vfwscanf(stdin, L"", a);
  va_end(a);
#endif
}

void print_stacktrace(int sig) {
  void *array[32];
  size_t size = backtrace(array, 32);
  brahma::test_logf_stream(stderr, "Error: signal %d\n", sig);
  backtrace_symbols_fd(array, size, STDERR_FILENO);
  exit(sig);
}

int main(int argc, char *argv[]) {
  signal(SIGSEGV, print_stacktrace);
  signal(SIGABRT, print_stacktrace);
  
  open("", 0);

  creat("", 0);

  creat64("", 0);

  open64("", 0);

  close(0);

  write(0, NULL, 0);

  read(0, NULL, 0);

  lseek(0, 0, 0);

  lseek64(0, 0, 0);

  pread(0, NULL, 0, 0);

  pread64(0, NULL, 0, 0);

  pwrite(0, NULL, 0, 0);

  pwrite64(0, NULL, 0, 0);

  fsync(0);

  fdatasync(0);

  openat(0, "", 0);
  openat64(0, "", 0);

  stat("", NULL);

  getcwd(NULL, 0);

  __open_2("", 0);

  __read_chk(0, NULL, 0, 0);

  __getcwd_chk(NULL, 0, 0);

  mkdir("", 0);

  rmdir("");

  chdir("");

  link("", "");

  linkat(01, "", 0, "", 0);

  unlink("");

  symlink("", "");

  symlinkat("", 0, "");

  readlink("", NULL, 0);

  readlinkat(0, "", NULL, 0);

  rename("", "");

  chmod("", 0);

  chown("", 0, 0);

  lchown("", 0, 0);

  utime("", NULL);

  // A single real DIR* is reused below for readdir/readdir64/rewinddir/
  // closedir/dirfd: each of those dereferences its DIR* argument directly
  // in userspace with no kernel-mediated safety net, so if any of them
  // isn't intercepted in a given environment, a NULL DIR* would be a real
  // crash. Reusing one opendir() call (rather than adding a second) keeps
  // opendir's own bound-vs-called count balanced.
  DIR *test_dir = opendir(".");

  readdir(test_dir);

  readdir64(test_dir);

  rewinddir(test_dir);
  closedir(test_dir);

  fcntl(0, 0);

  fcntl64(0, 0);

  dup(0);

  dup2(0, 0);

  dup3(0, 0, 0);

#if defined(__GLIBC__) && __GLIBC_PREREQ(2, 34)
  close_range(~0U, ~0U, 0);

  closefrom(~0U >> 1);
#endif

  pipe(NULL);

  mkfifo("", 0);

  umask(0);

  access("", 0);

  faccessat(0, "", 0, 0);

  remove("");

  truncate("", 0);

  truncate64("", 0);

  ftruncate(0, 0);

  ftruncate64(0, 0);

  execl("", "");

  execlp("", "");

  execv("", NULL);

  execvp("", NULL);

  execvpe("", NULL, NULL);

  fork();

  mmap(NULL, 0, 0, 0, 0, 0);
  
  mmap64(NULL, 0, 0, 0, 0, 0);
  
  munmap(NULL, 0);

  msync(NULL, 0, 0);

  sysconf(0);

  madvise(NULL, 0, 0);

  mprotect(NULL, 0, 0);

  mlock(NULL, 0);

  munlock(NULL, 0);

  mlockall(0);

  munlockall();

#if defined(__GLIBC__) && __GLIBC_PREREQ(2, 32)
  lstat("", NULL);
  // fd 0 is a real, valid descriptor (stdin), so if gotcha doesn't
  // intercept this call in a given environment, the real fstat() runs
  // instead; a NULL buf would be a real crash risk there (unlike a bad
  // path, which fails with ENOENT before touching buf).
  {
    struct stat st_buf;
    fstat(0, &st_buf);
  }
  fstatat(0, "", NULL, 0);
  stat64("", NULL);
  lstat64("", NULL);
  {
    struct stat64 st64_buf;
    fstat64(0, &st64_buf);
  }
  fstatat64(0, "", NULL, 0);
#endif
  posix_fadvise(0, 0, 0, 0);
  posix_fadvise64(0, 0, 0, 0);
  posix_fallocate(0, 0, 0);
  posix_fallocate64(0, 0, 0);
  flock(0, 0);
  readv(0, NULL, 0);
  writev(0, NULL, 0);
  preadv(0, NULL, 0, 0);
  preadv64(0, NULL, 0, 0);
  pwritev(0, NULL, 0, 0);
  pwritev64(0, NULL, 0, 0);
  renameat(0, "", 0, "");
  mkdirat(0, "", 0);
  unlinkat(0, "", 0);
  fchmodat(0, "", 0, 0);
  fchownat(0, "", 0, 0, 0);
  fchmod(0, 0);
  fchown(0, 0, 0);
  execve("", NULL, NULL);
  waitpid(0, NULL, 0);
  wait(NULL);
  realpath("", NULL);
  dirfd(NULL);
#if defined(__GLIBC__) && __GLIBC_PREREQ(2, 32)
  mknod("", 0, 0);
#endif
  sendfile(0, 0, NULL, 0);
  sendfile64(0, 0, NULL, 0);
  copy_file_range(0, NULL, 0, NULL, 0, 0);
  statvfs("", NULL);
  statvfs64("", NULL);
  // fd 0 is a real, valid descriptor; unlike fstat (a thin syscall wrapper
  // where the kernel safely returns EFAULT for a bad user pointer),
  // glibc's fstatvfs/fstatvfs64 are implemented in userspace and
  // dereference buf directly, so if not intercepted, a NULL buf here is a
  // real SIGSEGV.
  {
    struct statvfs vfs_buf;
    fstatvfs(0, &vfs_buf);
  }
  {
    struct statvfs64 vfs64_buf;
    fstatvfs64(0, &vfs64_buf);
  }

  preadv2(0, NULL, 0, 0, 0);
  pwritev2(0, NULL, 0, 0, 0);
  preadv64v2(0, NULL, 0, 0, 0);
  pwritev64v2(0, NULL, 0, 0, 0);
  splice(0, NULL, 0, NULL, 0, 0);
  tee(0, 0, 0, 0);
  vmsplice(0, NULL, 0, 0);
  fallocate(0, 0, 0, 0);
  fallocate64(0, 0, 0, 0);
  readahead(0, 0, 0);
  sync();
  syncfs(0);
  sync_file_range(0, 0, 0, 0);
  statx(0, "", 0, 0, NULL);
  statfs("", NULL);
  fstatfs(0, NULL);
  statfs64("", NULL);
  fstatfs64(0, NULL);
#if defined(__GLIBC__) && __GLIBC_PREREQ(2, 33)
  mknodat(0, "", 0, 0);
#endif
#if defined(__GLIBC__) && !__GLIBC_PREREQ(2, 33)
  __xmknodat(0, 0, "", 0, NULL);
#endif
  euidaccess("", 0);
  eaccess("", 0);
  renameat2(0, "", 0, "", 0);
  mkfifoat(0, "", 0);
  lchmod("", 0);
  utimes("", NULL);
  futimes(0, NULL);
  lutimes("", NULL);
  futimesat(0, "", NULL);
  utimensat(0, "", NULL, 0);
  futimens(0, NULL);
  fchdir(0);
  fdopendir(0);
  readdir_r(NULL, NULL, NULL);
  readdir64_r(NULL, NULL, NULL);
  seekdir(NULL, 0);
  telldir(NULL);
  scandir("", NULL, NULL, NULL);
  scandir64("", NULL, NULL, NULL);
  scandirat(0, "", NULL, NULL, NULL);
  scandirat64(0, "", NULL, NULL, NULL);
  nftw("", NULL, 0, 0);
  nftw64("", NULL, 0, 0);
#if defined(__GLIBC__) && __GLIBC_PREREQ(2, 30)
  getdents64(0, NULL, 0);
#endif
  mremap(NULL, 0, 0, 0);
  ioctl(0, 0, NULL);
  lockf(0, 0, 0);
  lockf64(0, 0, 0);
  pipe2(NULL, 0);
  memfd_create("", 0);
  mkstemp(NULL);
  mkstemp64(NULL);
  mkostemp(NULL, 0);
  mkostemp64(NULL, 0);
  mkstemps(NULL, 0);
  mkstemps64(NULL, 0);
  mkostemps(NULL, 0, 0);
  mkostemps64(NULL, 0, 0);
  mkdtemp(NULL);
  getxattr("", "", NULL, 0);
  lgetxattr("", "", NULL, 0);
  fgetxattr(0, "", NULL, 0);
  setxattr("", "", NULL, 0, 0);
  lsetxattr("", "", NULL, 0, 0);
  fsetxattr(0, "", NULL, 0, 0);
  listxattr("", NULL, 0);
  llistxattr("", NULL, 0);
  flistxattr(0, NULL, 0);
  removexattr("", "");
  lremovexattr("", "");
  fremovexattr(0, "");
  aio_read(NULL);
  aio_write(NULL);
  aio_error(NULL);
  aio_return(NULL);
  aio_fsync(0, NULL);
  aio_cancel(0, NULL);
  aio_suspend(NULL, 0, NULL);
  lio_listio(0, NULL, 0, NULL);
  aio_read64(NULL);
  aio_write64(NULL);
  aio_error64(NULL);
  aio_return64(NULL);
  aio_fsync64(0, NULL);
  aio_cancel64(0, NULL);
  aio_suspend64(NULL, 0, NULL);
  lio_listio64(0, NULL, 0, NULL);

  fopen("", "");
  fopen64("", "");
  fclose(NULL);
  fread(NULL, 0, 0, NULL);
  fwrite(NULL, 0, 0, NULL);
  ftell(NULL);
  fseek(NULL, 0, 0);
  fdopen(0, "");
  fileno(NULL);
  tmpfile();
  tmpfile64();
  fseeko(NULL, 0, 0);
  fseeko64(NULL, 0, 0);
  ftello(NULL);
  ftello64(NULL);

  clearerr(NULL);  
  feof(NULL);  
  ferror(NULL);  
  fflush(NULL);
  fgetc(NULL);
  fgetpos(NULL, 0);
  fgetpos64(NULL, 0);
  fgets(NULL, 1024, NULL);

  __fread_chk(NULL, 0, 0, 0, NULL);

  __fgets_chk(NULL, 0, 0, NULL);
  __fprintf_chk(stdout, 1, "");
  dprintf(1, "");
  __dprintf_chk(1, 1, "");
  fwrite_unlocked(NULL, 0, 0, NULL);
  fread_unlocked(NULL, 0, 0, NULL);
  fputs_unlocked(NULL, NULL);
  fgets_unlocked(NULL, 0, NULL);
  fputc_unlocked(0, NULL);
  fgetc_unlocked(NULL);
  fputc(0, NULL);
  fputs(NULL, NULL);
  fsetpos(NULL, NULL);
  fsetpos64(NULL, NULL);
  flockfile(NULL);
  ftrylockfile(NULL);
  funlockfile(NULL);
  getc(NULL);
  getc_unlocked(NULL);
  getw(NULL);
  pclose(NULL);
  putw(0, NULL);
  rewind(NULL);
  setvbuf(NULL, NULL, 0, 0);
  ungetc(0, NULL);

  // A real, non-NULL stream is used here: if not intercepted, the real
  // freopen/freopen64 would dereference stream directly.
  freopen("", "", stdin);
  // See STDIO::bind() in stdio.h: on platforms where glibc's __REDIRECT
  // aliases "freopen" to "freopen64", the call above already IS the one
  // and only call to freopen64 (the "freopen" binding is skipped there to
  // match), so a second, separate call here would double-count it. Only
  // make this second call when the two are genuinely distinct symbols.
  using freopen_sig = FILE *(*)(const char *, const char *, FILE *);
  if ((void *)(freopen_sig)&::freopen != (void *)(freopen_sig)&::freopen64) {
    freopen64("", "", stdin);
  }
  fprintf(stdout, "");
  printf("");
  {
    char buf[64];
    sprintf(buf, "");
    snprintf(buf, sizeof(buf), "");
  }
  fscanf(stdin, "");
  scanf("");
  sscanf("", "");
  __isoc99_fscanf(stdin, "");
  __isoc99_scanf("");
  __isoc99_sscanf("", "");
#if defined(__GLIBC__) && __GLIBC_PREREQ(2, 38)
  __isoc23_fscanf(stdin, "");
  __isoc23_scanf("");
  __isoc23_sscanf("", "");
#endif
  call_va_stdio_apis("");
  puts("");
  putchar(0);
  putc(0, NULL);
  putc_unlocked(0, NULL);
  getchar();
  getchar_unlocked();
  perror("");
  setbuf(NULL, NULL);
  setbuffer(NULL, NULL, 0);
  setlinebuf(NULL);
  {
    char *lineptr = NULL;
    size_t n = 0;
    getline(&lineptr, &n, NULL);
  }
  {
    char *lineptr = NULL;
    size_t n = 0;
    getdelim(&lineptr, &n, '\n', NULL);
  }
  fmemopen(NULL, 0, "");
  {
    char *ptr = NULL;
    size_t sz = 0;
    open_memstream(&ptr, &sz);
  }
  popen("", "");
  {
    char buf2[64];
    tmpnam(buf2);
  }

  fcloseall();
  {
    cookie_io_functions_t cookie_funcs = {NULL, NULL, NULL, NULL};
    fopencookie(NULL, "", cookie_funcs);
  }
  open_wmemstream(NULL, NULL);
  fflush_unlocked(NULL);
  feof_unlocked(NULL);
  ferror_unlocked(NULL);
  fileno_unlocked(NULL);
  clearerr_unlocked(NULL);
  _IO_getc(NULL);
  _IO_putc(0, NULL);
  fgetwc(NULL);
  getwc(NULL);
  fputwc(0, NULL);
  putwc(0, NULL);
  ungetwc(0, NULL);
  fgetws(NULL, 0, NULL);
  fputws(NULL, NULL);
  fwprintf(stdout, L"");
  __fwprintf_chk(stdout, 1, L"");
  fwscanf(stdin, L"");
  __isoc99_fwscanf(stdin, L"");
#if defined(__GLIBC__) && __GLIBC_PREREQ(2, 38)
  __isoc23_fwscanf(stdin, L"");
#endif

  exit(100);
  return 0;
}