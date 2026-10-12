//
// Created by hariharan on 8/8/22.
//

#ifndef BRAHMA_POSIX_H
#define BRAHMA_POSIX_H
#include <brahma/brahma_config.hpp>
/* Internal Headers */
#include <brahma/interceptor.h>
#include <brahma/interface/interface.h>
/* External Headers */
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <utime.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <sys/statvfs.h>
#include <sys/wait.h>
#include <sys/sendfile.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/vfs.h>
#include <sys/xattr.h>
#include <aio.h>
#include <ftw.h>
#include <features.h>

#include <cstdlib>
#include <cstring>
#include <stdexcept>

namespace brahma {
class POSIX : public Interface {
 private:
  static std::shared_ptr<POSIX> my_instance;

 public:
  static std::shared_ptr<POSIX> get_instance();

  POSIX() : Interface() {}

  virtual ~POSIX() {}

  template <typename C>
  size_t bind(const char *name, uint16_t priority);


  size_t unbind();

  static int set_instance(std::shared_ptr<POSIX> instance_i);

  virtual int open(const char *pathname, int flags, ...);

  virtual int creat(const char *path, mode_t mode);

  virtual int creat64(const char *path, mode_t mode);

  virtual int open64(const char *path, int flags, ...);

  virtual int close(int fd);

  virtual ssize_t write(int fd, const void *buf, size_t count);

  virtual ssize_t read(int fd, void *buf, size_t count);

  virtual off_t lseek(int fd, off_t offset, int whence);

  virtual off64_t lseek64(int fd, off64_t offset, int whence);

  virtual ssize_t pread(int fd, void *buf, size_t count, off_t offset);

  virtual ssize_t pread64(int fd, void *buf, size_t count, off64_t offset);

  virtual ssize_t pwrite(int fd, const void *buf, size_t count, off64_t offset);

  virtual ssize_t pwrite64(int fd, const void *buf, size_t count,
                           off64_t offset);

  virtual int fsync(int fd);

  virtual int fdatasync(int fd);

  virtual int openat(int dirfd, const char *pathname, int flags, ...);

  virtual int openat64(int dirfd, const char *pathname, int flags, ...);

  virtual int __xstat(int vers, const char *path, struct stat *buf);

  virtual int __xstat64(int vers, const char *path, struct stat64 *buf);

  virtual int __lxstat(int vers, const char *path, struct stat *buf);

  virtual int __lxstat64(int vers, const char *path, struct stat64 *buf);

  virtual int __fxstat(int vers, int fd, struct stat *buf);

  virtual int __fxstat64(int vers, int fd, struct stat64 *buf);

  virtual int __fxstatat(int vers, int dirfd, const char *path,
                         struct stat *buf, int flags);

  virtual int __fxstatat64(int vers, int dirfd, const char *path,
                           struct stat64 *buf, int flags);

  virtual int __xmknod(int vers, const char *path, mode_t mode, dev_t *dev);
  virtual int __open_2(const char *path, int oflag);
  virtual int __open64_2(const char *path, int oflag);
  virtual int __openat_2(int dirfd, const char *path, int oflag);
  virtual int __openat64_2(int dirfd, const char *path, int oflag);
  virtual ssize_t __read_chk(int fd, void *buf, size_t nbytes, size_t buflen);
  virtual ssize_t __pread_chk(int fd, void *buf, size_t nbytes, off_t offset, size_t buflen);
  virtual ssize_t __pread64_chk(int fd, void *buf, size_t nbytes, off64_t offset, size_t buflen);
  virtual ssize_t __readlink_chk(const char *path, char *buf, size_t len, size_t buflen);
  virtual ssize_t __readlinkat_chk(int dirfd, const char *path, char *buf, size_t len, size_t buflen);
  virtual char *__getcwd_chk(char *buf, size_t size, size_t buflen);
  virtual char *__realpath_chk(const char *path, char *resolved, size_t resolvedlen);

  virtual char *getcwd(char *buf, size_t size);

  virtual int mkdir(const char *pathname, mode_t mode);

  virtual int rmdir(const char *pathname);

  virtual int chdir(const char *path);

  virtual int link(const char *oldpath, const char *newpath);

  virtual int linkat(int fd1, const char *path1, int fd2, const char *path2,
                     int flag);

  virtual int unlink(const char *pathname);

  virtual int symlink(const char *path1, const char *path2);

  virtual int symlinkat(const char *path1, int fd, const char *path2);

  virtual ssize_t readlink(const char *path, char *buf, size_t bufsize);

  virtual ssize_t readlinkat(int fd, const char *path, char *buf,
                             size_t bufsize);

  virtual int rename(const char *oldpath, const char *newpath);

  virtual int chmod(const char *path, mode_t mode);

  virtual int chown(const char *path, uid_t owner, gid_t group);

  virtual int lchown(const char *path, uid_t owner, gid_t group);

  virtual int utime(const char *filename, const utimbuf *buf);

  virtual DIR *opendir(const char *name);

  virtual dirent *readdir(DIR *dir);

  virtual dirent64 *readdir64(DIR *dir);

  virtual int closedir(DIR *dir);

  virtual void rewinddir(DIR *dir);

  virtual int fcntl(int fd, int cmd, ...);

  virtual int fcntl64(int fd, int cmd, ...);

  virtual int dup(int oldfd);

  virtual int dup2(int oldfd, int newfd);

  virtual int dup3(int oldfd, int newfd, int flags);

  virtual int close_range(unsigned int first, unsigned int last, int flags);

  virtual void closefrom(int lowfd);

  virtual int pipe(int pipefd[2]);

  virtual int mkfifo(const char *pathname, mode_t mode);

  virtual mode_t umask(mode_t mask);

  virtual int access(const char *path, int amode);

  virtual int faccessat(int fd, const char *path, int amode, int flag);

  virtual int remove(const char *pathname);

  virtual int truncate(const char *pathname, off_t length);

  virtual int truncate64(const char *pathname, off64_t length);

  virtual int ftruncate(int fd, off_t length);

  virtual int ftruncate64(int fd, off64_t length);

  virtual int execl(const char *pathname, const char *arg, ...);

  virtual int execlp(const char *file, const char *arg, ...);

  virtual int execv(const char *pathname, char *const argv[]);

  virtual int execvp(const char *file, char *const argv[]);

  virtual int execvpe(const char *file, char *const argv[], char *const envp[]);

  virtual int fork();

  virtual void exit(int status);
  
  virtual void _exit(int status);

  virtual void *mmap(void *addr, size_t length, int prot, int flags, int fd,
                     off_t offset);

  virtual void *mmap64(void *addr, size_t length, int prot, int flags, int fd,
                       off64_t offset);
  
  virtual int munmap(void *addr, size_t len);

  virtual int msync(void *addr, size_t len, int flags);

  virtual long sysconf(int name);

  virtual int madvise(void *addr, size_t length, int advice);

  virtual int mprotect(void *addr, size_t length, int prot);

  virtual int mlock(const void *addr, size_t len);
  
  virtual int munlock(const void *addr, size_t len);

  virtual int mlockall(int flags);

  virtual int munlockall(void);

  virtual void _fini(void);

  // Before glibc 2.33 these are not exported dynamic symbols (calls go
  // through __xstat/__lxstat/__fxstat, wrapped above), so binding the plain
  // names is a no-op there. They stay unguarded so a build on an old glibc
  // still intercepts them when run on a newer one.
  virtual int stat(const char *path, struct stat *buf);

  virtual int lstat(const char *path, struct stat *buf);

  virtual int fstat(int fd, struct stat *buf);

  virtual int fstatat(int dirfd, const char *path, struct stat *buf,
                      int flags);

  // On platforms where glibc's _FILE_OFFSET_BITS=64 __REDIRECT makes
  // stat/lstat/fstat/fstatat the same underlying symbol as their "64"
  // counterpart (renamed at the linker level), calls in application code
  // resolve directly to these instead -- both names must be bound to
  // guarantee interception either way (see GOTCHA_BINDING_MACRO_LFS64).
  virtual int stat64(const char *path, struct stat64 *buf);

  virtual int lstat64(const char *path, struct stat64 *buf);

  virtual int fstat64(int fd, struct stat64 *buf);

  virtual int fstatat64(int dirfd, const char *path, struct stat64 *buf,
                        int flags);

  virtual int posix_fadvise(int fd, off_t offset, off_t len, int advice);

  virtual int posix_fadvise64(int fd, off64_t offset, off64_t len, int advice);

  virtual int posix_fallocate(int fd, off_t offset, off_t len);

  virtual int posix_fallocate64(int fd, off64_t offset, off64_t len);

  virtual int flock(int fd, int operation);

  virtual ssize_t readv(int fd, const struct iovec *iov, int iovcnt);

  virtual ssize_t writev(int fd, const struct iovec *iov, int iovcnt);

  virtual ssize_t preadv(int fd, const struct iovec *iov, int iovcnt,
                         off_t offset);

  virtual ssize_t preadv64(int fd, const struct iovec *iov, int iovcnt,
                           off64_t offset);

  virtual ssize_t pwritev(int fd, const struct iovec *iov, int iovcnt,
                          off_t offset);

  virtual ssize_t pwritev64(int fd, const struct iovec *iov, int iovcnt,
                            off64_t offset);

  virtual int renameat(int olddirfd, const char *oldpath, int newdirfd,
                       const char *newpath);

  virtual int mkdirat(int dirfd, const char *pathname, mode_t mode);

  virtual int unlinkat(int dirfd, const char *pathname, int flags);

  virtual int fchmodat(int dirfd, const char *pathname, mode_t mode,
                       int flags);

  virtual int fchownat(int dirfd, const char *pathname, uid_t owner,
                       gid_t group, int flags);

  virtual int fchmod(int fd, mode_t mode);

  virtual int fchown(int fd, uid_t owner, gid_t group);

  virtual int execve(const char *pathname, char *const argv[],
                     char *const envp[]);

  virtual pid_t waitpid(pid_t pid, int *wstatus, int options);

  virtual pid_t wait(int *wstatus);

  virtual char *realpath(const char *path, char *resolved_path);

  virtual int dirfd(DIR *dir);

  // See stat/lstat/fstat/fstatat above: same pre-2.33 __xmknod indirection.
  virtual int mknod(const char *pathname, mode_t mode, dev_t dev);

  virtual ssize_t sendfile(int out_fd, int in_fd, off_t *offset,
                           size_t count);

  virtual ssize_t sendfile64(int out_fd, int in_fd, off64_t *offset,
                             size_t count);

  virtual ssize_t copy_file_range(int fd_in, off64_t *off_in, int fd_out,
                                  off64_t *off_out, size_t len,
                                  unsigned int flags);

  virtual int statvfs(const char *path, struct statvfs *buf);

  virtual int statvfs64(const char *path, struct statvfs64 *buf);

  virtual int fstatvfs(int fd, struct statvfs *buf);

  virtual int fstatvfs64(int fd, struct statvfs64 *buf);

  virtual ssize_t preadv2(int fd, const struct iovec *iov, int iovcnt, off_t offset, int flags);

  virtual ssize_t pwritev2(int fd, const struct iovec *iov, int iovcnt, off_t offset, int flags);

  virtual ssize_t preadv64v2(int fd, const struct iovec *iov, int iovcnt, off64_t offset, int flags);

  virtual ssize_t pwritev64v2(int fd, const struct iovec *iov, int iovcnt, off64_t offset, int flags);

  virtual ssize_t splice(int fd_in, off64_t *off_in, int fd_out, off64_t *off_out, size_t len, unsigned int flags);

  virtual ssize_t tee(int fd_in, int fd_out, size_t len, unsigned int flags);

  virtual ssize_t vmsplice(int fd, const struct iovec *iov, size_t nr_segs, unsigned int flags);

  virtual int fallocate(int fd, int mode, off_t offset, off_t len);

  virtual int fallocate64(int fd, int mode, off64_t offset, off64_t len);

  virtual ssize_t readahead(int fd, off64_t offset, size_t count);

  virtual void sync(void);

  virtual int syncfs(int fd);

  virtual int sync_file_range(int fd, off64_t offset, off64_t nbytes, unsigned int flags);

  virtual int statx(int dirfd, const char *path, int flags, unsigned int mask, struct statx *buf);

  virtual int statfs(const char *path, struct statfs *buf);

  virtual int fstatfs(int fd, struct statfs *buf);

  virtual int statfs64(const char *path, struct statfs64 *buf);

  virtual int fstatfs64(int fd, struct statfs64 *buf);

  virtual int mknodat(int dirfd, const char *path, mode_t mode, dev_t dev);

  virtual int __xmknodat(int ver, int dirfd, const char *path, mode_t mode, dev_t *dev);

  virtual int euidaccess(const char *path, int mode);

  virtual int eaccess(const char *path, int mode);

  virtual int renameat2(int olddirfd, const char *oldpath, int newdirfd, const char *newpath, unsigned int flags);

  virtual int mkfifoat(int dirfd, const char *path, mode_t mode);

  virtual int lchmod(const char *path, mode_t mode);

  virtual int utimes(const char *path, const struct timeval times[2]);

  virtual int futimes(int fd, const struct timeval tv[2]);

  virtual int lutimes(const char *path, const struct timeval tv[2]);

  virtual int futimesat(int dirfd, const char *path, const struct timeval times[2]);

  virtual int utimensat(int dirfd, const char *path, const struct timespec times[2], int flags);

  virtual int futimens(int fd, const struct timespec times[2]);

  virtual int fchdir(int fd);

  virtual DIR *fdopendir(int fd);

  virtual int readdir_r(DIR *dir, struct dirent *entry, struct dirent **res);

  virtual int readdir64_r(DIR *dir, struct dirent64 *entry, struct dirent64 **res);

  virtual void seekdir(DIR *dir, long loc);

  virtual long telldir(DIR *dir);

  virtual int scandir(const char *dir, struct dirent ***namelist, int (*filter)(const struct dirent *), int (*compar)(const struct dirent **, const struct dirent **));

  virtual int scandir64(const char *dir, struct dirent64 ***namelist, int (*filter)(const struct dirent64 *), int (*compar)(const struct dirent64 **, const struct dirent64 **));

  virtual int scandirat(int dirfd, const char *dir, struct dirent ***namelist, int (*filter)(const struct dirent *), int (*compar)(const struct dirent **, const struct dirent **));
  virtual int scandirat64(int dirfd, const char *dir, struct dirent64 ***namelist, int (*filter)(const struct dirent64 *), int (*compar)(const struct dirent64 **, const struct dirent64 **));

  virtual int nftw(const char *path, int (*func)(const char *, const struct stat *, int, struct FTW *), int nopenfd, int flags);

  virtual int nftw64(const char *path, int (*func)(const char *, const struct stat64 *, int, struct FTW *), int nopenfd, int flags);

  virtual ssize_t getdents64(int fd, void *buf, size_t count);

  virtual void *mremap(void *old_addr, size_t old_size, size_t new_size, int flags, void *new_addr);

  virtual int ioctl(int fd, unsigned long request, void *arg);

  virtual int lockf(int fd, int cmd, off_t len);

  virtual int lockf64(int fd, int cmd, off64_t len);

  virtual int pipe2(int pipefd[2], int flags);

  virtual int memfd_create(const char *name, unsigned int flags);

  virtual int mkstemp(char *tmpl);

  virtual int mkstemp64(char *tmpl);

  virtual int mkostemp(char *tmpl, int flags);

  virtual int mkostemp64(char *tmpl, int flags);

  virtual int mkstemps(char *tmpl, int suffixlen);

  virtual int mkstemps64(char *tmpl, int suffixlen);

  virtual int mkostemps(char *tmpl, int suffixlen, int flags);

  virtual int mkostemps64(char *tmpl, int suffixlen, int flags);

  virtual char *mkdtemp(char *tmpl);

  virtual ssize_t getxattr(const char *path, const char *name, void *value, size_t size);

  virtual ssize_t lgetxattr(const char *path, const char *name, void *value, size_t size);

  virtual ssize_t fgetxattr(int fd, const char *name, void *value, size_t size);

  virtual int setxattr(const char *path, const char *name, const void *value, size_t size, int flags);

  virtual int lsetxattr(const char *path, const char *name, const void *value, size_t size, int flags);

  virtual int fsetxattr(int fd, const char *name, const void *value, size_t size, int flags);

  virtual ssize_t listxattr(const char *path, char *list, size_t size);

  virtual ssize_t llistxattr(const char *path, char *list, size_t size);

  virtual ssize_t flistxattr(int fd, char *list, size_t size);

  virtual int removexattr(const char *path, const char *name);

  virtual int lremovexattr(const char *path, const char *name);

  virtual int fremovexattr(int fd, const char *name);

  virtual int aio_read(struct aiocb *cb);

  virtual int aio_write(struct aiocb *cb);

  virtual int aio_error(const struct aiocb *cb);

  virtual ssize_t aio_return(struct aiocb *cb);

  virtual int aio_fsync(int op, struct aiocb *cb);

  virtual int aio_cancel(int fd, struct aiocb *cb);

  virtual int aio_suspend(const struct aiocb *const list[], int nent, const struct timespec *timeout);

  virtual int lio_listio(int mode, struct aiocb *const list[], int nent, struct sigevent *sig);

  virtual int aio_read64(struct aiocb64 *cb);

  virtual int aio_write64(struct aiocb64 *cb);

  virtual int aio_error64(const struct aiocb64 *cb);

  virtual ssize_t aio_return64(struct aiocb64 *cb);

  virtual int aio_fsync64(int op, struct aiocb64 *cb);

  virtual int aio_cancel64(int fd, struct aiocb64 *cb);

  virtual int aio_suspend64(const struct aiocb64 *const list[], int nent, const struct timespec *timeout);

  virtual int lio_listio64(int mode, struct aiocb64 *const list[], int nent, struct sigevent *sig);

  /* Handler Definitions */
  GOTCHA_MACRO_VAR(open)
  GOTCHA_MACRO_VAR(creat)
  GOTCHA_MACRO_VAR(creat64)
  GOTCHA_MACRO_VAR(open64)
  GOTCHA_MACRO_VAR(close)
  GOTCHA_MACRO_VAR(write)
  GOTCHA_MACRO_VAR(read)
  GOTCHA_MACRO_VAR(lseek)
  GOTCHA_MACRO_VAR(lseek64)
  GOTCHA_MACRO_VAR(pread)
  GOTCHA_MACRO_VAR(pread64)
  GOTCHA_MACRO_VAR(pwrite)
  GOTCHA_MACRO_VAR(pwrite64)
  GOTCHA_MACRO_VAR(fsync)
  GOTCHA_MACRO_VAR(fdatasync)
  GOTCHA_MACRO_VAR(openat)
  GOTCHA_MACRO_VAR(openat64)
  GOTCHA_MACRO_VAR(__xstat)
  GOTCHA_MACRO_VAR(__xstat64)
  GOTCHA_MACRO_VAR(__lxstat)
  GOTCHA_MACRO_VAR(__lxstat64)
  GOTCHA_MACRO_VAR(__fxstat)
  GOTCHA_MACRO_VAR(__fxstat64)
  GOTCHA_MACRO_VAR(__fxstatat)
  GOTCHA_MACRO_VAR(__fxstatat64)
  GOTCHA_MACRO_VAR(__xmknod)
  GOTCHA_MACRO_VAR(__open_2)
  GOTCHA_MACRO_VAR(__open64_2)
  GOTCHA_MACRO_VAR(__openat_2)
  GOTCHA_MACRO_VAR(__openat64_2)
  GOTCHA_MACRO_VAR(__read_chk)
  GOTCHA_MACRO_VAR(__pread_chk)
  GOTCHA_MACRO_VAR(__pread64_chk)
  GOTCHA_MACRO_VAR(__readlink_chk)
  GOTCHA_MACRO_VAR(__readlinkat_chk)
  GOTCHA_MACRO_VAR(__getcwd_chk)
  GOTCHA_MACRO_VAR(__realpath_chk)
  GOTCHA_MACRO_VAR(getcwd)
  GOTCHA_MACRO_VAR(mkdir)
  GOTCHA_MACRO_VAR(rmdir)
  GOTCHA_MACRO_VAR(chdir)
  GOTCHA_MACRO_VAR(link)
  GOTCHA_MACRO_VAR(linkat)
  GOTCHA_MACRO_VAR(unlink)
  GOTCHA_MACRO_VAR(symlink)
  GOTCHA_MACRO_VAR(symlinkat)
  GOTCHA_MACRO_VAR(readlink)
  GOTCHA_MACRO_VAR(readlinkat)
  GOTCHA_MACRO_VAR(rename)
  GOTCHA_MACRO_VAR(chmod)
  GOTCHA_MACRO_VAR(chown)
  GOTCHA_MACRO_VAR(lchown)
  GOTCHA_MACRO_VAR(utime)
  GOTCHA_MACRO_VAR(opendir)
  GOTCHA_MACRO_VAR(readdir)
  GOTCHA_MACRO_VAR(readdir64)
  GOTCHA_MACRO_VAR(closedir)
  GOTCHA_MACRO_VAR(rewinddir)
  GOTCHA_MACRO_VAR(fcntl)
  GOTCHA_MACRO_VAR(fcntl64)
  GOTCHA_MACRO_VAR(dup)
  GOTCHA_MACRO_VAR(dup2)
  GOTCHA_MACRO_VAR(dup3)
  GOTCHA_MACRO_VAR(close_range)
  GOTCHA_MACRO_VAR(closefrom)
  GOTCHA_MACRO_VAR(pipe)
  GOTCHA_MACRO_VAR(mkfifo)
  GOTCHA_MACRO_VAR(umask)
  GOTCHA_MACRO_VAR(access)
  GOTCHA_MACRO_VAR(faccessat)
  GOTCHA_MACRO_VAR(remove)
  GOTCHA_MACRO_VAR(truncate)
  GOTCHA_MACRO_VAR(truncate64)
  GOTCHA_MACRO_VAR(ftruncate)
  GOTCHA_MACRO_VAR(ftruncate64)
  GOTCHA_MACRO_VAR(execl)
  GOTCHA_MACRO_VAR(execlp)
  GOTCHA_MACRO_VAR(execv)
  GOTCHA_MACRO_VAR(execvp)
  GOTCHA_MACRO_VAR(execvpe)
  GOTCHA_MACRO_VAR(fork)
  GOTCHA_MACRO_VAR(exit)
  GOTCHA_MACRO_VAR(_exit)
  GOTCHA_MACRO_VAR(mmap)
  GOTCHA_MACRO_VAR(mmap64)
  GOTCHA_MACRO_VAR(munmap)
  GOTCHA_MACRO_VAR(msync)
  GOTCHA_MACRO_VAR(sysconf)
  GOTCHA_MACRO_VAR(madvise)
  GOTCHA_MACRO_VAR(mprotect)
  GOTCHA_MACRO_VAR(mlock)
  GOTCHA_MACRO_VAR(munlock)
  GOTCHA_MACRO_VAR(mlockall)
  GOTCHA_MACRO_VAR(munlockall)
  GOTCHA_MACRO_VAR(_fini)
  GOTCHA_MACRO_VAR(stat)
  GOTCHA_MACRO_VAR(lstat)
  GOTCHA_MACRO_VAR(fstat)
  GOTCHA_MACRO_VAR(fstatat)
  GOTCHA_MACRO_VAR(stat64)
  GOTCHA_MACRO_VAR(lstat64)
  GOTCHA_MACRO_VAR(fstat64)
  GOTCHA_MACRO_VAR(fstatat64)
  GOTCHA_MACRO_VAR(posix_fadvise)
  GOTCHA_MACRO_VAR(posix_fadvise64)
  GOTCHA_MACRO_VAR(posix_fallocate)
  GOTCHA_MACRO_VAR(posix_fallocate64)
  GOTCHA_MACRO_VAR(flock)
  GOTCHA_MACRO_VAR(readv)
  GOTCHA_MACRO_VAR(writev)
  GOTCHA_MACRO_VAR(preadv)
  GOTCHA_MACRO_VAR(preadv64)
  GOTCHA_MACRO_VAR(pwritev)
  GOTCHA_MACRO_VAR(pwritev64)
  GOTCHA_MACRO_VAR(renameat)
  GOTCHA_MACRO_VAR(mkdirat)
  GOTCHA_MACRO_VAR(unlinkat)
  GOTCHA_MACRO_VAR(fchmodat)
  GOTCHA_MACRO_VAR(fchownat)
  GOTCHA_MACRO_VAR(fchmod)
  GOTCHA_MACRO_VAR(fchown)
  GOTCHA_MACRO_VAR(execve)
  GOTCHA_MACRO_VAR(waitpid)
  GOTCHA_MACRO_VAR(wait)
  GOTCHA_MACRO_VAR(realpath)
  GOTCHA_MACRO_VAR(dirfd)
  GOTCHA_MACRO_VAR(mknod)
  GOTCHA_MACRO_VAR(sendfile)
  GOTCHA_MACRO_VAR(sendfile64)
  GOTCHA_MACRO_VAR(copy_file_range)
  GOTCHA_MACRO_VAR(statvfs)
  GOTCHA_MACRO_VAR(statvfs64)
  GOTCHA_MACRO_VAR(fstatvfs)
  GOTCHA_MACRO_VAR(fstatvfs64)
  GOTCHA_MACRO_VAR(preadv2)
  GOTCHA_MACRO_VAR(pwritev2)
  GOTCHA_MACRO_VAR(preadv64v2)
  GOTCHA_MACRO_VAR(pwritev64v2)
  GOTCHA_MACRO_VAR(splice)
  GOTCHA_MACRO_VAR(tee)
  GOTCHA_MACRO_VAR(vmsplice)
  GOTCHA_MACRO_VAR(fallocate)
  GOTCHA_MACRO_VAR(fallocate64)
  GOTCHA_MACRO_VAR(readahead)
  GOTCHA_MACRO_VAR(sync)
  GOTCHA_MACRO_VAR(syncfs)
  GOTCHA_MACRO_VAR(sync_file_range)
  GOTCHA_MACRO_VAR(statx)
  GOTCHA_MACRO_VAR(statfs)
  GOTCHA_MACRO_VAR(fstatfs)
  GOTCHA_MACRO_VAR(statfs64)
  GOTCHA_MACRO_VAR(fstatfs64)
  GOTCHA_MACRO_VAR(mknodat)
  GOTCHA_MACRO_VAR(__xmknodat)
  GOTCHA_MACRO_VAR(euidaccess)
  GOTCHA_MACRO_VAR(eaccess)
  GOTCHA_MACRO_VAR(renameat2)
  GOTCHA_MACRO_VAR(mkfifoat)
  GOTCHA_MACRO_VAR(lchmod)
  GOTCHA_MACRO_VAR(utimes)
  GOTCHA_MACRO_VAR(futimes)
  GOTCHA_MACRO_VAR(lutimes)
  GOTCHA_MACRO_VAR(futimesat)
  GOTCHA_MACRO_VAR(utimensat)
  GOTCHA_MACRO_VAR(futimens)
  GOTCHA_MACRO_VAR(fchdir)
  GOTCHA_MACRO_VAR(fdopendir)
  GOTCHA_MACRO_VAR(readdir_r)
  GOTCHA_MACRO_VAR(readdir64_r)
  GOTCHA_MACRO_VAR(seekdir)
  GOTCHA_MACRO_VAR(telldir)
  GOTCHA_MACRO_VAR(scandir)
  GOTCHA_MACRO_VAR(scandir64)
  GOTCHA_MACRO_VAR(scandirat)
  GOTCHA_MACRO_VAR(scandirat64)
  GOTCHA_MACRO_VAR(nftw)
  GOTCHA_MACRO_VAR(nftw64)
  GOTCHA_MACRO_VAR(getdents64)
  GOTCHA_MACRO_VAR(mremap)
  GOTCHA_MACRO_VAR(ioctl)
  GOTCHA_MACRO_VAR(lockf)
  GOTCHA_MACRO_VAR(lockf64)
  GOTCHA_MACRO_VAR(pipe2)
  GOTCHA_MACRO_VAR(memfd_create)
  GOTCHA_MACRO_VAR(mkstemp)
  GOTCHA_MACRO_VAR(mkstemp64)
  GOTCHA_MACRO_VAR(mkostemp)
  GOTCHA_MACRO_VAR(mkostemp64)
  GOTCHA_MACRO_VAR(mkstemps)
  GOTCHA_MACRO_VAR(mkstemps64)
  GOTCHA_MACRO_VAR(mkostemps)
  GOTCHA_MACRO_VAR(mkostemps64)
  GOTCHA_MACRO_VAR(mkdtemp)
  GOTCHA_MACRO_VAR(getxattr)
  GOTCHA_MACRO_VAR(lgetxattr)
  GOTCHA_MACRO_VAR(fgetxattr)
  GOTCHA_MACRO_VAR(setxattr)
  GOTCHA_MACRO_VAR(lsetxattr)
  GOTCHA_MACRO_VAR(fsetxattr)
  GOTCHA_MACRO_VAR(listxattr)
  GOTCHA_MACRO_VAR(llistxattr)
  GOTCHA_MACRO_VAR(flistxattr)
  GOTCHA_MACRO_VAR(removexattr)
  GOTCHA_MACRO_VAR(lremovexattr)
  GOTCHA_MACRO_VAR(fremovexattr)
  GOTCHA_MACRO_VAR(aio_read)
  GOTCHA_MACRO_VAR(aio_write)
  GOTCHA_MACRO_VAR(aio_error)
  GOTCHA_MACRO_VAR(aio_return)
  GOTCHA_MACRO_VAR(aio_fsync)
  GOTCHA_MACRO_VAR(aio_cancel)
  GOTCHA_MACRO_VAR(aio_suspend)
  GOTCHA_MACRO_VAR(lio_listio)
  GOTCHA_MACRO_VAR(aio_read64)
  GOTCHA_MACRO_VAR(aio_write64)
  GOTCHA_MACRO_VAR(aio_error64)
  GOTCHA_MACRO_VAR(aio_return64)
  GOTCHA_MACRO_VAR(aio_fsync64)
  GOTCHA_MACRO_VAR(aio_cancel64)
  GOTCHA_MACRO_VAR(aio_suspend64)
  GOTCHA_MACRO_VAR(lio_listio64)
};

}  // namespace brahma

GOTCHA_MACRO_TYPEDEF_OPEN(open, int, (const char *pathname, int flags, ...),
                          (pathname, flags, mode), flags, brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(creat, int, (const char *path, mode_t mode),
                     (path, mode), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(creat64, int, (const char *path, mode_t mode),
                     (path, mode), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF_OPEN(open64, int, (const char *path, int flags, ...),
                          (path, flags, mode), flags, brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(close, int, (int fd), (fd), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(write, ssize_t, (int fd, const void *buf, size_t count),
                     (fd, buf, count), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(read, ssize_t, (int fd, void *buf, size_t count),
                     (fd, buf, count), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(lseek, off_t, (int fd, off_t offset, int whence),
                     (fd, offset, whence), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(lseek64, off64_t, (int fd, off64_t offset, int whence),
                     (fd, offset, whence), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(pread, ssize_t,
                     (int fd, void *buf, size_t count, off_t offset),
                     (fd, buf, count, offset), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(pread64, ssize_t,
                     (int fd, void *buf, size_t count, off64_t offset),
                     (fd, buf, count, offset), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(pwrite, ssize_t,
                     (int fd, const void *buf, size_t count, off_t offset),
                     (fd, buf, count, offset), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(pwrite64, ssize_t,
                     (int fd, const void *buf, size_t count, off64_t offset),
                     (fd, buf, count, offset), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(fsync, int, (int fd), (fd), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(fdatasync, int, (int fd), (fd), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF_OPEN(openat, int,
                          (int dirfd, const char *pathname, int flags, ...),
                          (dirfd, pathname, flags, mode), flags, brahma::POSIX)
GOTCHA_MACRO_TYPEDEF_OPEN(openat64, int,
                     (int dirfd, const char *pathname, int flags, ...),
                     (dirfd, pathname, flags, mode), flags, brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(__xstat, int,
                     (int vers, const char *path, struct stat *buf),
                     (vers, path, buf), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(__xstat64, int,
                     (int vers, const char *path, struct stat64 *buf),
                     (vers, path, buf), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(__lxstat, int,
                     (int vers, const char *path, struct stat *buf),
                     (vers, path, buf), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(__lxstat64, int,
                     (int vers, const char *path, struct stat64 *buf),
                     (vers, path, buf), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(__fxstat, int, (int vers, int fd, struct stat *buf),
                     (vers, fd, buf), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(__fxstat64, int, (int vers, int fd, struct stat64 *buf),
                     (vers, fd, buf), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(__fxstatat, int,
                     (int vers, int dirfd, const char *path, struct stat *buf,
                      int flags),
                     (vers, dirfd, path, buf, flags), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(__fxstatat64, int,
                     (int vers, int dirfd, const char *path,
                      struct stat64 *buf, int flags),
                     (vers, dirfd, path, buf, flags), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(__xmknod, int,
                     (int vers, const char *path, mode_t mode, dev_t *dev),
                     (vers, path, mode, dev), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF_C(__open_2, int, (const char *path, int oflag),
                       (path, oflag), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF_C(__open64_2, int, (const char *path, int oflag),
                       (path, oflag), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF_C(__openat_2, int, (int dirfd, const char *path, int oflag),
                       (dirfd, path, oflag), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF_C(__openat64_2, int, (int dirfd, const char *path, int oflag),
                       (dirfd, path, oflag), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF_C(__read_chk, ssize_t, (int fd, void *buf, size_t nbytes, size_t buflen),
                       (fd, buf, nbytes, buflen), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF_C(__pread_chk, ssize_t, (int fd, void *buf, size_t nbytes, off_t offset, size_t buflen),
                       (fd, buf, nbytes, offset, buflen), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF_C(__pread64_chk, ssize_t, (int fd, void *buf, size_t nbytes, off64_t offset, size_t buflen),
                       (fd, buf, nbytes, offset, buflen), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF_C(__readlink_chk, ssize_t, (const char *path, char *buf, size_t len, size_t buflen),
                       (path, buf, len, buflen), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF_C(__readlinkat_chk, ssize_t, (int dirfd, const char *path, char *buf, size_t len, size_t buflen),
                       (dirfd, path, buf, len, buflen), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF_C(__getcwd_chk, char *, (char *buf, size_t size, size_t buflen),
                       (buf, size, buflen), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF_C(__realpath_chk, char *, (const char *path, char *resolved, size_t resolvedlen),
                       (path, resolved, resolvedlen), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(getcwd, char *, (char *buf, size_t size), (buf, size),
                     brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(mkdir, int, (const char *pathname, mode_t mode),
                     (pathname, mode), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(rmdir, int, (const char *pathname), (pathname),
                     brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(chdir, int, (const char *path), (path), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(link, int, (const char *oldpath, const char *newpath),
                     (oldpath, newpath), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(linkat, int,
                     (int fd1, const char *path1, int fd2, const char *path2,
                      int flag),
                     (fd1, path1, fd2, path2, flag), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(unlink, int, (const char *pathname), (pathname),
                     brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(symlink, int, (const char *path1, const char *path2),
                     (path1, path2), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(symlinkat, int,
                     (const char *path1, int fd, const char *path2),
                     (path1, fd, path2), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(readlink, ssize_t,
                     (const char *path, char *buf, size_t bufsize),
                     (path, buf, bufsize), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(readlinkat, ssize_t,
                     (int fd, const char *path, char *buf, size_t bufsize),
                     (fd, path, buf, bufsize), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(rename, int, (const char *oldpath, const char *newpath),
                     (oldpath, newpath), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(chmod, int, (const char *path, mode_t mode), (path, mode),
                     brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(chown, int, (const char *path, uid_t owner, gid_t group),
                     (path, owner, group), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(lchown, int, (const char *path, uid_t owner, gid_t group),
                     (path, owner, group), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(utime, int, (const char *filename, const utimbuf *buf),
                     (filename, buf), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(opendir, DIR *, (const char *name), (name), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(readdir, struct dirent *, (DIR * dir), (dir),
                     brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(readdir64, struct dirent64 *, (DIR * dir), (dir),
                     brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(closedir, int, (DIR * dir), (dir), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(rewinddir, void, (DIR * dir), (dir), brahma::POSIX)
typedef int (*fcntl_fptr)(int fd, int cmd, ...);
inline int fcntl_wrapper(int fd, int cmd, ...) {
  if (cmd == F_DUPFD || cmd == F_DUPFD_CLOEXEC || cmd == F_SETFD ||
      cmd == F_SETFL || cmd == F_SETOWN) {  // arg: int
    va_list arg;
    va_start(arg, cmd);
    int val = va_arg(arg, int);
    va_end(arg);
    int v = brahma::POSIX::get_instance()->fcntl(fd, cmd, val);
    return v;
  } else if (cmd == F_GETFD || cmd == F_GETFL || cmd == F_GETOWN) {
    int v = brahma::POSIX::get_instance()->fcntl(fd, cmd);
    return v;
  } else if (cmd == F_SETLK || cmd == F_SETLKW || cmd == F_GETLK) {
    va_list arg;
    va_start(arg, cmd);
    struct flock *lk = va_arg(arg, struct flock *);
    va_end(arg);
    int v = brahma::POSIX::get_instance()->fcntl(fd, cmd, lk);
    return v;
  } else {  // assume arg: void, cmd==F_GETOWN_EX || cmd==F_SETOWN_EX
            // ||cmd==F_GETSIG || cmd==F_SETSIG)
    int v = brahma::POSIX::get_instance()->fcntl(fd, cmd);
    return v;
  }
}
gotcha_wrappee_handle_t get_fcntl_handle();
typedef int (*fcntl64_fptr)(int fd, int cmd, ...);
inline int fcntl64_wrapper(int fd, int cmd, ...) {
  if (cmd == F_DUPFD || cmd == F_DUPFD_CLOEXEC || cmd == F_SETFD ||
      cmd == F_SETFL || cmd == F_SETOWN) {  // arg: int
    va_list arg;
    va_start(arg, cmd);
    int val = va_arg(arg, int);
    va_end(arg);
    int v = brahma::POSIX::get_instance()->fcntl64(fd, cmd, val);
    return v;
  } else if (cmd == F_GETFD || cmd == F_GETFL || cmd == F_GETOWN) {
    int v = brahma::POSIX::get_instance()->fcntl64(fd, cmd);
    return v;
  } else if (cmd == F_SETLK || cmd == F_SETLKW || cmd == F_GETLK) {
    va_list arg;
    va_start(arg, cmd);
    struct flock *lk = va_arg(arg, struct flock *);
    va_end(arg);
    int v = brahma::POSIX::get_instance()->fcntl64(fd, cmd, lk);
    return v;
  } else {  // assume arg: void, cmd==F_GETOWN_EX || cmd==F_SETOWN_EX
            // ||cmd==F_GETSIG || cmd==F_SETSIG)
    int v = brahma::POSIX::get_instance()->fcntl64(fd, cmd);
    return v;
  }
}
gotcha_wrappee_handle_t get_fcntl64_handle();
GOTCHA_MACRO_TYPEDEF(dup, int, (int oldfd), (oldfd), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(dup2, int, (int oldfd, int newfd), (oldfd, newfd),
                     brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(dup3, int, (int oldfd, int newfd, int flags),
                     (oldfd, newfd, flags), brahma::POSIX)
// glibc 2.34 added these, so they use the asm alias to build on older headers.
GOTCHA_MACRO_TYPEDEF_ALIAS(close_range, int,
                           (unsigned int first, unsigned int last, int flags),
                           (first, last, flags), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF_ALIAS(closefrom, void, (int lowfd), (lowfd), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(pipe, int, (int pipefd[2]), (pipefd), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(mkfifo, int, (const char *pathname, mode_t mode),
                     (pathname, mode), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(umask, mode_t, (mode_t mask), (mask), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(access, int, (const char *path, int amode), (path, amode),
                     brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(faccessat, int,
                     (int fd, const char *path, int amode, int flag),
                     (fd, path, amode, flag), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(remove, int, (const char *pathname), (pathname),
                     brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(truncate, int, (const char *pathname, off_t length),
                     (pathname, length), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(truncate64, int, (const char *pathname, off64_t length),
                     (pathname, length), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(ftruncate, int, (int fd, off_t length), (fd, length),
                     brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(ftruncate64, int, (int fd, off64_t length), (fd, length),
                     brahma::POSIX)
GOTCHA_MACRO_TYPEDEF_EXECL(execl, int,
                           (const char *pathname, const char *arg, ...),
                           (pathname, arg, val), arg, brahma::POSIX)
GOTCHA_MACRO_TYPEDEF_EXECL(execlp, int,
                           (const char *pathname, const char *arg, ...),
                           (pathname, arg, val), arg, brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(execv, int, (const char *pathname, char *const argv[]),
                     (pathname, argv), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(execvp, int, (const char *pathname, char *const argv[]),
                     (pathname, argv), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(execvpe, int,
                     (const char *pathname, char *const argv[],
                      char *const envp[]),
                     (pathname, argv, envp), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(fork, int, (), (), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(exit, void,
                     (int status),
                     (status), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(_exit, void,
                     (int status),
                     (status), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(mmap, void *,
                     (void *addr, size_t length, int prot, int flags, int fd,
                      off_t offset),
                     (addr, length, prot, flags, fd, offset), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(mmap64, void *,
                     (void *addr, size_t length, int prot, int flags, int fd,
                      off64_t offset),
                     (addr, length, prot, flags, fd, offset), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(munmap, int,
                     (void *addr, size_t len),
                     (addr, len), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(msync, int,
                     (void *addr, size_t len, int flags),
                     (addr, len, flags), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(sysconf, long,
                     (int name),
                     (name), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(madvise, int,
                     (void *addr, size_t length, int advice),
                     (addr, length, advice), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(mprotect, int,
                     (void *addr, size_t length, int prot),
                     (addr, length, prot), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(mlock, int,
                     (const void *addr, size_t length),
                     (addr, length), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(munlock, int,
                     (const void *addr, size_t length),
                     (addr, length), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(mlockall, int,
                     (int flags),
                     (flags), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(munlockall, int, (), (), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(_fini, void, (void), (), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF_ALIAS(stat, int, (const char *path, struct stat *buf),
                     (path, buf), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF_ALIAS(lstat, int, (const char *path, struct stat *buf),
                     (path, buf), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF_ALIAS(fstat, int, (int fd, struct stat *buf), (fd, buf),
                     brahma::POSIX)
GOTCHA_MACRO_TYPEDEF_ALIAS(fstatat, int,
                     (int dirfd, const char *path, struct stat *buf,
                      int flags),
                     (dirfd, path, buf, flags), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF_ALIAS(stat64, int, (const char *path, struct stat64 *buf),
                     (path, buf), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF_ALIAS(lstat64, int, (const char *path, struct stat64 *buf),
                     (path, buf), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF_ALIAS(fstat64, int, (int fd, struct stat64 *buf), (fd, buf),
                     brahma::POSIX)
GOTCHA_MACRO_TYPEDEF_ALIAS(fstatat64, int,
                     (int dirfd, const char *path, struct stat64 *buf,
                      int flags),
                     (dirfd, path, buf, flags), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(posix_fadvise, int,
                     (int fd, off_t offset, off_t len, int advice),
                     (fd, offset, len, advice), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(posix_fadvise64, int,
                     (int fd, off64_t offset, off64_t len, int advice),
                     (fd, offset, len, advice), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(posix_fallocate, int,
                     (int fd, off_t offset, off_t len),
                     (fd, offset, len), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(posix_fallocate64, int,
                     (int fd, off64_t offset, off64_t len),
                     (fd, offset, len), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(flock, int, (int fd, int operation), (fd, operation),
                     brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(readv, ssize_t,
                     (int fd, const struct iovec *iov, int iovcnt),
                     (fd, iov, iovcnt), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(writev, ssize_t,
                     (int fd, const struct iovec *iov, int iovcnt),
                     (fd, iov, iovcnt), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(preadv, ssize_t,
                     (int fd, const struct iovec *iov, int iovcnt,
                      off_t offset),
                     (fd, iov, iovcnt, offset), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(preadv64, ssize_t,
                     (int fd, const struct iovec *iov, int iovcnt,
                      off64_t offset),
                     (fd, iov, iovcnt, offset), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(pwritev, ssize_t,
                     (int fd, const struct iovec *iov, int iovcnt,
                      off_t offset),
                     (fd, iov, iovcnt, offset), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(pwritev64, ssize_t,
                     (int fd, const struct iovec *iov, int iovcnt,
                      off64_t offset),
                     (fd, iov, iovcnt, offset), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(renameat, int,
                     (int olddirfd, const char *oldpath, int newdirfd,
                      const char *newpath),
                     (olddirfd, oldpath, newdirfd, newpath), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(mkdirat, int,
                     (int dirfd, const char *pathname, mode_t mode),
                     (dirfd, pathname, mode), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(unlinkat, int,
                     (int dirfd, const char *pathname, int flags),
                     (dirfd, pathname, flags), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(fchmodat, int,
                     (int dirfd, const char *pathname, mode_t mode,
                      int flags),
                     (dirfd, pathname, mode, flags), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(fchownat, int,
                     (int dirfd, const char *pathname, uid_t owner,
                      gid_t group, int flags),
                     (dirfd, pathname, owner, group, flags), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(fchmod, int, (int fd, mode_t mode), (fd, mode),
                     brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(fchown, int, (int fd, uid_t owner, gid_t group),
                     (fd, owner, group), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(execve, int,
                     (const char *pathname, char *const argv[],
                      char *const envp[]),
                     (pathname, argv, envp), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(waitpid, pid_t,
                     (pid_t pid, int *wstatus, int options),
                     (pid, wstatus, options), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(wait, pid_t, (int *wstatus), (wstatus), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(realpath, char *,
                     (const char *path, char *resolved_path),
                     (path, resolved_path), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(dirfd, int, (DIR * dir), (dir), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF_ALIAS(mknod, int,
                     (const char *pathname, mode_t mode, dev_t dev),
                     (pathname, mode, dev), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(sendfile, ssize_t,
                     (int out_fd, int in_fd, off_t *offset, size_t count),
                     (out_fd, in_fd, offset, count), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(sendfile64, ssize_t,
                     (int out_fd, int in_fd, off64_t *offset, size_t count),
                     (out_fd, in_fd, offset, count), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(copy_file_range, ssize_t,
                     (int fd_in, off64_t *off_in, int fd_out,
                      off64_t *off_out, size_t len, unsigned int flags),
                     (fd_in, off_in, fd_out, off_out, len, flags),
                     brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(statvfs, int, (const char *path, struct statvfs *buf),
                     (path, buf), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(statvfs64, int,
                     (const char *path, struct statvfs64 *buf),
                     (path, buf), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(fstatvfs, int, (int fd, struct statvfs *buf), (fd, buf),
                     brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(fstatvfs64, int, (int fd, struct statvfs64 *buf),
                     (fd, buf), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(preadv2, ssize_t,
                     (int fd, const struct iovec *iov, int iovcnt, off_t offset, int flags),
                     (fd, iov, iovcnt, offset, flags), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(pwritev2, ssize_t,
                     (int fd, const struct iovec *iov, int iovcnt, off_t offset, int flags),
                     (fd, iov, iovcnt, offset, flags), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(preadv64v2, ssize_t,
                     (int fd, const struct iovec *iov, int iovcnt, off64_t offset, int flags),
                     (fd, iov, iovcnt, offset, flags), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(pwritev64v2, ssize_t,
                     (int fd, const struct iovec *iov, int iovcnt, off64_t offset, int flags),
                     (fd, iov, iovcnt, offset, flags), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(splice, ssize_t,
                     (int fd_in, off64_t *off_in, int fd_out, off64_t *off_out, size_t len, unsigned int flags),
                     (fd_in, off_in, fd_out, off_out, len, flags), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(tee, ssize_t,
                     (int fd_in, int fd_out, size_t len, unsigned int flags),
                     (fd_in, fd_out, len, flags), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(vmsplice, ssize_t,
                     (int fd, const struct iovec *iov, size_t nr_segs, unsigned int flags),
                     (fd, iov, nr_segs, flags), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(fallocate, int,
                     (int fd, int mode, off_t offset, off_t len),
                     (fd, mode, offset, len), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(fallocate64, int,
                     (int fd, int mode, off64_t offset, off64_t len),
                     (fd, mode, offset, len), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(readahead, ssize_t,
                     (int fd, off64_t offset, size_t count),
                     (fd, offset, count), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(sync, void,
                     (void),
                     (), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(syncfs, int,
                     (int fd),
                     (fd), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(sync_file_range, int,
                     (int fd, off64_t offset, off64_t nbytes, unsigned int flags),
                     (fd, offset, nbytes, flags), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(statx, int,
                     (int dirfd, const char *path, int flags, unsigned int mask, struct statx *buf),
                     (dirfd, path, flags, mask, buf), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(statfs, int,
                     (const char *path, struct statfs *buf),
                     (path, buf), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(fstatfs, int,
                     (int fd, struct statfs *buf),
                     (fd, buf), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(statfs64, int,
                     (const char *path, struct statfs64 *buf),
                     (path, buf), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(fstatfs64, int,
                     (int fd, struct statfs64 *buf),
                     (fd, buf), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF_ALIAS(mknodat, int,
                           (int dirfd, const char *path, mode_t mode, dev_t dev),
                           (dirfd, path, mode, dev), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(__xmknodat, int,
                     (int ver, int dirfd, const char *path, mode_t mode, dev_t *dev),
                     (ver, dirfd, path, mode, dev), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(euidaccess, int,
                     (const char *path, int mode),
                     (path, mode), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(eaccess, int,
                     (const char *path, int mode),
                     (path, mode), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(renameat2, int,
                     (int olddirfd, const char *oldpath, int newdirfd, const char *newpath, unsigned int flags),
                     (olddirfd, oldpath, newdirfd, newpath, flags), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(mkfifoat, int,
                     (int dirfd, const char *path, mode_t mode),
                     (dirfd, path, mode), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(lchmod, int,
                     (const char *path, mode_t mode),
                     (path, mode), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(utimes, int,
                     (const char *path, const struct timeval times[2]),
                     (path, times), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(futimes, int,
                     (int fd, const struct timeval tv[2]),
                     (fd, tv), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(lutimes, int,
                     (const char *path, const struct timeval tv[2]),
                     (path, tv), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(futimesat, int,
                     (int dirfd, const char *path, const struct timeval times[2]),
                     (dirfd, path, times), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(utimensat, int,
                     (int dirfd, const char *path, const struct timespec times[2], int flags),
                     (dirfd, path, times, flags), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(futimens, int,
                     (int fd, const struct timespec times[2]),
                     (fd, times), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(fchdir, int,
                     (int fd),
                     (fd), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(fdopendir, DIR *,
                     (int fd),
                     (fd), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(readdir_r, int,
                     (DIR *dir, struct dirent *entry, struct dirent **res),
                     (dir, entry, res), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(readdir64_r, int,
                     (DIR *dir, struct dirent64 *entry, struct dirent64 **res),
                     (dir, entry, res), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(seekdir, void,
                     (DIR *dir, long loc),
                     (dir, loc), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(telldir, long,
                     (DIR *dir),
                     (dir), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(scandir, int,
                     (const char *dir, struct dirent ***namelist, int (*filter)(const struct dirent *), int (*compar)(const struct dirent **, const struct dirent **)),
                     (dir, namelist, filter, compar), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(scandir64, int,
                     (const char *dir, struct dirent64 ***namelist, int (*filter)(const struct dirent64 *), int (*compar)(const struct dirent64 **, const struct dirent64 **)),
                     (dir, namelist, filter, compar), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(scandirat, int,
                     (int dirfd, const char *dir, struct dirent ***namelist, int (*filter)(const struct dirent *), int (*compar)(const struct dirent **, const struct dirent **)),
                     (dirfd, dir, namelist, filter, compar), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(scandirat64, int,
                     (int dirfd, const char *dir, struct dirent64 ***namelist, int (*filter)(const struct dirent64 *), int (*compar)(const struct dirent64 **, const struct dirent64 **)),
                     (dirfd, dir, namelist, filter, compar), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(nftw, int,
                     (const char *path, int (*func)(const char *, const struct stat *, int, struct FTW *), int nopenfd, int flags),
                     (path, func, nopenfd, flags), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(nftw64, int,
                     (const char *path, int (*func)(const char *, const struct stat64 *, int, struct FTW *), int nopenfd, int flags),
                     (path, func, nopenfd, flags), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF_ALIAS(getdents64, ssize_t,
                           (int fd, void *buf, size_t count),
                           (fd, buf, count), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(lockf, int,
                     (int fd, int cmd, off_t len),
                     (fd, cmd, len), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(lockf64, int,
                     (int fd, int cmd, off64_t len),
                     (fd, cmd, len), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(pipe2, int,
                     (int pipefd[2], int flags),
                     (pipefd, flags), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(memfd_create, int,
                     (const char *name, unsigned int flags),
                     (name, flags), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(mkstemp, int,
                     (char *tmpl),
                     (tmpl), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(mkstemp64, int,
                     (char *tmpl),
                     (tmpl), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(mkostemp, int,
                     (char *tmpl, int flags),
                     (tmpl, flags), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(mkostemp64, int,
                     (char *tmpl, int flags),
                     (tmpl, flags), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(mkstemps, int,
                     (char *tmpl, int suffixlen),
                     (tmpl, suffixlen), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(mkstemps64, int,
                     (char *tmpl, int suffixlen),
                     (tmpl, suffixlen), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(mkostemps, int,
                     (char *tmpl, int suffixlen, int flags),
                     (tmpl, suffixlen, flags), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(mkostemps64, int,
                     (char *tmpl, int suffixlen, int flags),
                     (tmpl, suffixlen, flags), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(mkdtemp, char *,
                     (char *tmpl),
                     (tmpl), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(getxattr, ssize_t,
                     (const char *path, const char *name, void *value, size_t size),
                     (path, name, value, size), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(lgetxattr, ssize_t,
                     (const char *path, const char *name, void *value, size_t size),
                     (path, name, value, size), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(fgetxattr, ssize_t,
                     (int fd, const char *name, void *value, size_t size),
                     (fd, name, value, size), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(setxattr, int,
                     (const char *path, const char *name, const void *value, size_t size, int flags),
                     (path, name, value, size, flags), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(lsetxattr, int,
                     (const char *path, const char *name, const void *value, size_t size, int flags),
                     (path, name, value, size, flags), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(fsetxattr, int,
                     (int fd, const char *name, const void *value, size_t size, int flags),
                     (fd, name, value, size, flags), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(listxattr, ssize_t,
                     (const char *path, char *list, size_t size),
                     (path, list, size), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(llistxattr, ssize_t,
                     (const char *path, char *list, size_t size),
                     (path, list, size), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(flistxattr, ssize_t,
                     (int fd, char *list, size_t size),
                     (fd, list, size), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(removexattr, int,
                     (const char *path, const char *name),
                     (path, name), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(lremovexattr, int,
                     (const char *path, const char *name),
                     (path, name), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(fremovexattr, int,
                     (int fd, const char *name),
                     (fd, name), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(aio_read, int,
                     (struct aiocb *cb),
                     (cb), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(aio_write, int,
                     (struct aiocb *cb),
                     (cb), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(aio_error, int,
                     (const struct aiocb *cb),
                     (cb), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(aio_return, ssize_t,
                     (struct aiocb *cb),
                     (cb), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(aio_fsync, int,
                     (int op, struct aiocb *cb),
                     (op, cb), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(aio_cancel, int,
                     (int fd, struct aiocb *cb),
                     (fd, cb), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(aio_suspend, int,
                     (const struct aiocb *const list[], int nent, const struct timespec *timeout),
                     (list, nent, timeout), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(lio_listio, int,
                     (int mode, struct aiocb *const list[], int nent, struct sigevent *sig),
                     (mode, list, nent, sig), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(aio_read64, int,
                     (struct aiocb64 *cb),
                     (cb), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(aio_write64, int,
                     (struct aiocb64 *cb),
                     (cb), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(aio_error64, int,
                     (const struct aiocb64 *cb),
                     (cb), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(aio_return64, ssize_t,
                     (struct aiocb64 *cb),
                     (cb), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(aio_fsync64, int,
                     (int op, struct aiocb64 *cb),
                     (op, cb), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(aio_cancel64, int,
                     (int fd, struct aiocb64 *cb),
                     (fd, cb), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(aio_suspend64, int,
                     (const struct aiocb64 *const list[], int nent, const struct timespec *timeout),
                     (list, nent, timeout), brahma::POSIX)
GOTCHA_MACRO_TYPEDEF(lio_listio64, int,
                     (int mode, struct aiocb64 *const list[], int nent, struct sigevent *sig),
                     (mode, list, nent, sig), brahma::POSIX)
typedef void *(*mremap_fptr)(void *old_addr, size_t old_size, size_t new_size,
                             int flags, ...);
inline void *mremap_wrapper(void *old_addr, size_t old_size, size_t new_size,
                            int flags, ...) {
  void *new_addr = nullptr;
  if (flags & MREMAP_FIXED) {
    va_list arg;
    va_start(arg, flags);
    new_addr = va_arg(arg, void *);
    va_end(arg);
  }
  return brahma::POSIX::get_instance()->mremap(old_addr, old_size, new_size,
                                               flags, new_addr);
}
typedef int (*ioctl_fptr)(int fd, unsigned long request, ...);
inline int ioctl_wrapper(int fd, unsigned long request, ...) {
  va_list arg;
  va_start(arg, request);
  void *val = va_arg(arg, void *);
  va_end(arg);
  return brahma::POSIX::get_instance()->ioctl(fd, request, val);
}

template <typename C>
size_t brahma::POSIX::bind(const char *name, uint16_t priority) {
  GOTCHA_BINDING_MACRO(open, POSIX);
  GOTCHA_BINDING_MACRO(creat, POSIX);
  GOTCHA_BINDING_MACRO(creat64, POSIX);
  GOTCHA_BINDING_MACRO(open64, POSIX);
  GOTCHA_BINDING_MACRO(close, POSIX);
  GOTCHA_BINDING_MACRO(write, POSIX);
  GOTCHA_BINDING_MACRO(read, POSIX);
  GOTCHA_BINDING_MACRO(lseek, POSIX);
  GOTCHA_BINDING_MACRO(lseek64, POSIX);
  GOTCHA_BINDING_MACRO(pread, POSIX);
  GOTCHA_BINDING_MACRO(pread64, POSIX);
  GOTCHA_BINDING_MACRO(pwrite, POSIX);
  GOTCHA_BINDING_MACRO(pwrite64, POSIX);
  GOTCHA_BINDING_MACRO(fsync, POSIX);
  GOTCHA_BINDING_MACRO(fdatasync, POSIX);
  GOTCHA_BINDING_MACRO(openat, POSIX);
  GOTCHA_BINDING_MACRO(openat64, POSIX);
  GOTCHA_BINDING_MACRO(__xstat, POSIX);
  GOTCHA_BINDING_MACRO(__xstat64, POSIX);
  GOTCHA_BINDING_MACRO(__lxstat, POSIX);
  GOTCHA_BINDING_MACRO(__lxstat64, POSIX);
  GOTCHA_BINDING_MACRO(__fxstat, POSIX);
  GOTCHA_BINDING_MACRO(__fxstat64, POSIX);
  GOTCHA_BINDING_MACRO(__fxstatat, POSIX);
  GOTCHA_BINDING_MACRO(__fxstatat64, POSIX);
  GOTCHA_BINDING_MACRO(__xmknod, POSIX);
  GOTCHA_BINDING_MACRO(__open_2, POSIX);
  GOTCHA_BINDING_MACRO(__open64_2, POSIX);
  GOTCHA_BINDING_MACRO(__openat_2, POSIX);
  GOTCHA_BINDING_MACRO(__openat64_2, POSIX);
  GOTCHA_BINDING_MACRO(__read_chk, POSIX);
  GOTCHA_BINDING_MACRO(__pread_chk, POSIX);
  GOTCHA_BINDING_MACRO(__pread64_chk, POSIX);
  GOTCHA_BINDING_MACRO(__readlink_chk, POSIX);
  GOTCHA_BINDING_MACRO(__readlinkat_chk, POSIX);
  GOTCHA_BINDING_MACRO(__getcwd_chk, POSIX);
  GOTCHA_BINDING_MACRO(__realpath_chk, POSIX);
  GOTCHA_BINDING_MACRO(getcwd, POSIX);
  GOTCHA_BINDING_MACRO(mkdir, POSIX);
  GOTCHA_BINDING_MACRO(rmdir, POSIX);
  GOTCHA_BINDING_MACRO(chdir, POSIX);
  GOTCHA_BINDING_MACRO(link, POSIX);
  GOTCHA_BINDING_MACRO(linkat, POSIX);
  GOTCHA_BINDING_MACRO(unlink, POSIX);
  GOTCHA_BINDING_MACRO(symlink, POSIX);
  GOTCHA_BINDING_MACRO(symlinkat, POSIX);
  GOTCHA_BINDING_MACRO(readlink, POSIX);
  GOTCHA_BINDING_MACRO(readlinkat, POSIX);
  GOTCHA_BINDING_MACRO(rename, POSIX);
  GOTCHA_BINDING_MACRO(chmod, POSIX);
  GOTCHA_BINDING_MACRO(chown, POSIX);
  GOTCHA_BINDING_MACRO(lchown, POSIX);
  GOTCHA_BINDING_MACRO(utime, POSIX);
  GOTCHA_BINDING_MACRO(opendir, POSIX);
  GOTCHA_BINDING_MACRO(readdir, POSIX);
  GOTCHA_BINDING_MACRO(readdir64, POSIX);
  GOTCHA_BINDING_MACRO(closedir, POSIX);
  GOTCHA_BINDING_MACRO(rewinddir, POSIX);
  GOTCHA_BINDING_MACRO(fcntl, POSIX);
  GOTCHA_BINDING_MACRO(fcntl64, POSIX);
  GOTCHA_BINDING_MACRO(dup, POSIX);
  GOTCHA_BINDING_MACRO(dup2, POSIX);
  GOTCHA_BINDING_MACRO(dup3, POSIX);
  GOTCHA_BINDING_MACRO_ALIAS(close_range, POSIX);
  GOTCHA_BINDING_MACRO_ALIAS(closefrom, POSIX);
  GOTCHA_BINDING_MACRO(pipe, POSIX);
  GOTCHA_BINDING_MACRO(mkfifo, POSIX);
  GOTCHA_BINDING_MACRO(umask, POSIX);
  GOTCHA_BINDING_MACRO(access, POSIX);
  GOTCHA_BINDING_MACRO(faccessat, POSIX);
  GOTCHA_BINDING_MACRO(remove, POSIX);
  GOTCHA_BINDING_MACRO(truncate, POSIX);
  GOTCHA_BINDING_MACRO(truncate64, POSIX);
  GOTCHA_BINDING_MACRO(ftruncate, POSIX);
  GOTCHA_BINDING_MACRO(ftruncate64, POSIX);
  GOTCHA_BINDING_MACRO(execl, POSIX);
  GOTCHA_BINDING_MACRO(execlp, POSIX);
  GOTCHA_BINDING_MACRO(execv, POSIX);
  GOTCHA_BINDING_MACRO(execvp, POSIX);
  GOTCHA_BINDING_MACRO(execvpe, POSIX);
  GOTCHA_BINDING_MACRO(fork, POSIX);
  GOTCHA_BINDING_MACRO(exit, POSIX);
  GOTCHA_BINDING_MACRO(_exit, POSIX);
  GOTCHA_BINDING_MACRO(mmap, POSIX);
  GOTCHA_BINDING_MACRO(mmap64, POSIX);
  GOTCHA_BINDING_MACRO(munmap, POSIX);
  GOTCHA_BINDING_MACRO(msync, POSIX);
  GOTCHA_BINDING_MACRO(sysconf, POSIX);
  GOTCHA_BINDING_MACRO(madvise, POSIX);
  GOTCHA_BINDING_MACRO(mprotect, POSIX);
  GOTCHA_BINDING_MACRO(mlock, POSIX);
  GOTCHA_BINDING_MACRO(munlock, POSIX);
  GOTCHA_BINDING_MACRO(mlockall, POSIX);
  GOTCHA_BINDING_MACRO(munlockall, POSIX);
  GOTCHA_BINDING_MACRO(_fini, POSIX);
  GOTCHA_BINDING_MACRO_ALIAS(stat, POSIX);
  GOTCHA_BINDING_MACRO_ALIAS(lstat, POSIX);
  GOTCHA_BINDING_MACRO_ALIAS(fstat, POSIX);
  GOTCHA_BINDING_MACRO_ALIAS(fstatat, POSIX);
  GOTCHA_BINDING_MACRO_ALIAS(stat64, POSIX);
  GOTCHA_BINDING_MACRO_ALIAS(lstat64, POSIX);
  GOTCHA_BINDING_MACRO_ALIAS(fstat64, POSIX);
  GOTCHA_BINDING_MACRO_ALIAS(fstatat64, POSIX);
  GOTCHA_BINDING_MACRO(posix_fadvise, POSIX);
  GOTCHA_BINDING_MACRO(posix_fadvise64, POSIX);
  GOTCHA_BINDING_MACRO(posix_fallocate, POSIX);
  GOTCHA_BINDING_MACRO(posix_fallocate64, POSIX);
  GOTCHA_BINDING_MACRO(flock, POSIX);
  GOTCHA_BINDING_MACRO(readv, POSIX);
  GOTCHA_BINDING_MACRO(writev, POSIX);
  GOTCHA_BINDING_MACRO(preadv, POSIX);
  GOTCHA_BINDING_MACRO(preadv64, POSIX);
  GOTCHA_BINDING_MACRO(pwritev, POSIX);
  GOTCHA_BINDING_MACRO(pwritev64, POSIX);
  GOTCHA_BINDING_MACRO(renameat, POSIX);
  GOTCHA_BINDING_MACRO(mkdirat, POSIX);
  GOTCHA_BINDING_MACRO(unlinkat, POSIX);
  GOTCHA_BINDING_MACRO(fchmodat, POSIX);
  GOTCHA_BINDING_MACRO(fchownat, POSIX);
  GOTCHA_BINDING_MACRO(fchmod, POSIX);
  GOTCHA_BINDING_MACRO(fchown, POSIX);
  GOTCHA_BINDING_MACRO(execve, POSIX);
  GOTCHA_BINDING_MACRO(waitpid, POSIX);
  GOTCHA_BINDING_MACRO(wait, POSIX);
  GOTCHA_BINDING_MACRO(realpath, POSIX);
  GOTCHA_BINDING_MACRO(dirfd, POSIX);
  GOTCHA_BINDING_MACRO_ALIAS(mknod, POSIX);
  GOTCHA_BINDING_MACRO(sendfile, POSIX);
  GOTCHA_BINDING_MACRO(sendfile64, POSIX);
  GOTCHA_BINDING_MACRO(copy_file_range, POSIX);
  GOTCHA_BINDING_MACRO(statvfs, POSIX);
  GOTCHA_BINDING_MACRO(statvfs64, POSIX);
  GOTCHA_BINDING_MACRO(fstatvfs, POSIX);
  GOTCHA_BINDING_MACRO(fstatvfs64, POSIX);
  GOTCHA_BINDING_MACRO(preadv2, POSIX);
  GOTCHA_BINDING_MACRO(pwritev2, POSIX);
  GOTCHA_BINDING_MACRO(preadv64v2, POSIX);
  GOTCHA_BINDING_MACRO(pwritev64v2, POSIX);
  GOTCHA_BINDING_MACRO(splice, POSIX);
  GOTCHA_BINDING_MACRO(tee, POSIX);
  GOTCHA_BINDING_MACRO(vmsplice, POSIX);
  GOTCHA_BINDING_MACRO(fallocate, POSIX);
  GOTCHA_BINDING_MACRO(fallocate64, POSIX);
  GOTCHA_BINDING_MACRO(readahead, POSIX);
  GOTCHA_BINDING_MACRO(sync, POSIX);
  GOTCHA_BINDING_MACRO(syncfs, POSIX);
  GOTCHA_BINDING_MACRO(sync_file_range, POSIX);
  GOTCHA_BINDING_MACRO(statx, POSIX);
  GOTCHA_BINDING_MACRO(statfs, POSIX);
  GOTCHA_BINDING_MACRO(fstatfs, POSIX);
  GOTCHA_BINDING_MACRO(statfs64, POSIX);
  GOTCHA_BINDING_MACRO(fstatfs64, POSIX);
  GOTCHA_BINDING_MACRO_ALIAS(mknodat, POSIX);
  GOTCHA_BINDING_MACRO(__xmknodat, POSIX);
  GOTCHA_BINDING_MACRO(euidaccess, POSIX);
  GOTCHA_BINDING_MACRO(eaccess, POSIX);
  GOTCHA_BINDING_MACRO(renameat2, POSIX);
  GOTCHA_BINDING_MACRO(mkfifoat, POSIX);
  GOTCHA_BINDING_MACRO(lchmod, POSIX);
  GOTCHA_BINDING_MACRO(utimes, POSIX);
  GOTCHA_BINDING_MACRO(futimes, POSIX);
  GOTCHA_BINDING_MACRO(lutimes, POSIX);
  GOTCHA_BINDING_MACRO(futimesat, POSIX);
  GOTCHA_BINDING_MACRO(utimensat, POSIX);
  GOTCHA_BINDING_MACRO(futimens, POSIX);
  GOTCHA_BINDING_MACRO(fchdir, POSIX);
  GOTCHA_BINDING_MACRO(fdopendir, POSIX);
  GOTCHA_BINDING_MACRO(readdir_r, POSIX);
  GOTCHA_BINDING_MACRO(readdir64_r, POSIX);
  GOTCHA_BINDING_MACRO(seekdir, POSIX);
  GOTCHA_BINDING_MACRO(telldir, POSIX);
  GOTCHA_BINDING_MACRO(scandir, POSIX);
  GOTCHA_BINDING_MACRO(scandir64, POSIX);
  GOTCHA_BINDING_MACRO(scandirat, POSIX);
  GOTCHA_BINDING_MACRO(scandirat64, POSIX);
  GOTCHA_BINDING_MACRO(nftw, POSIX);
  GOTCHA_BINDING_MACRO(nftw64, POSIX);
  GOTCHA_BINDING_MACRO_ALIAS(getdents64, POSIX);
  GOTCHA_BINDING_MACRO(mremap, POSIX);
  GOTCHA_BINDING_MACRO(ioctl, POSIX);
  GOTCHA_BINDING_MACRO(lockf, POSIX);
  GOTCHA_BINDING_MACRO(lockf64, POSIX);
  GOTCHA_BINDING_MACRO(pipe2, POSIX);
  GOTCHA_BINDING_MACRO(memfd_create, POSIX);
  GOTCHA_BINDING_MACRO(mkstemp, POSIX);
  GOTCHA_BINDING_MACRO(mkstemp64, POSIX);
  GOTCHA_BINDING_MACRO(mkostemp, POSIX);
  GOTCHA_BINDING_MACRO(mkostemp64, POSIX);
  GOTCHA_BINDING_MACRO(mkstemps, POSIX);
  GOTCHA_BINDING_MACRO(mkstemps64, POSIX);
  GOTCHA_BINDING_MACRO(mkostemps, POSIX);
  GOTCHA_BINDING_MACRO(mkostemps64, POSIX);
  GOTCHA_BINDING_MACRO(mkdtemp, POSIX);
  GOTCHA_BINDING_MACRO(getxattr, POSIX);
  GOTCHA_BINDING_MACRO(lgetxattr, POSIX);
  GOTCHA_BINDING_MACRO(fgetxattr, POSIX);
  GOTCHA_BINDING_MACRO(setxattr, POSIX);
  GOTCHA_BINDING_MACRO(lsetxattr, POSIX);
  GOTCHA_BINDING_MACRO(fsetxattr, POSIX);
  GOTCHA_BINDING_MACRO(listxattr, POSIX);
  GOTCHA_BINDING_MACRO(llistxattr, POSIX);
  GOTCHA_BINDING_MACRO(flistxattr, POSIX);
  GOTCHA_BINDING_MACRO(removexattr, POSIX);
  GOTCHA_BINDING_MACRO(lremovexattr, POSIX);
  GOTCHA_BINDING_MACRO(fremovexattr, POSIX);
  GOTCHA_BINDING_MACRO(aio_read, POSIX);
  GOTCHA_BINDING_MACRO(aio_write, POSIX);
  GOTCHA_BINDING_MACRO(aio_error, POSIX);
  GOTCHA_BINDING_MACRO(aio_return, POSIX);
  GOTCHA_BINDING_MACRO(aio_fsync, POSIX);
  GOTCHA_BINDING_MACRO(aio_cancel, POSIX);
  GOTCHA_BINDING_MACRO(aio_suspend, POSIX);
  GOTCHA_BINDING_MACRO(lio_listio, POSIX);
  GOTCHA_BINDING_MACRO(aio_read64, POSIX);
  GOTCHA_BINDING_MACRO(aio_write64, POSIX);
  GOTCHA_BINDING_MACRO(aio_error64, POSIX);
  GOTCHA_BINDING_MACRO(aio_return64, POSIX);
  GOTCHA_BINDING_MACRO(aio_fsync64, POSIX);
  GOTCHA_BINDING_MACRO(aio_cancel64, POSIX);
  GOTCHA_BINDING_MACRO(aio_suspend64, POSIX);
  GOTCHA_BINDING_MACRO(lio_listio64, POSIX);
  num_bindings = bindings.size();
  if (num_bindings > 0) {
    // sprintf/snprintf are themselves interceptable POSIX/STDIO functions;
    // using them here would self-trigger whatever override is active for
    // them. strcpy/strcat are never bound, so they're safe for this
    // internal bookkeeping.
    strcpy(tool_name, name);
    strcat(tool_name, "_posix");
    gotcha_binding_t *raw_bindings = bindings.data();
    gotcha_wrap(raw_bindings, num_bindings, tool_name);
    bind_priority = priority;
    gotcha_set_priority(tool_name, priority);
  }
  return num_bindings;
}

#endif  // BRAHMA_POSIX_H
