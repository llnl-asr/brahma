//
// Created by hariharan on 8/16/22.
//
#include <brahma/interface/posix.h>
#include <fcntl.h>
namespace brahma {

std::shared_ptr<POSIX> POSIX::my_instance = nullptr;

std::shared_ptr<POSIX> POSIX::get_instance() {
  if (my_instance == nullptr) {
    BRAHMA_LOG_INFO("POSIX class not intercepted but used", "");
    my_instance = std::make_shared<POSIX>();
  }
  return my_instance;
}
int POSIX::set_instance(std::shared_ptr<POSIX> instance_i) {
  if (instance_i != nullptr) {
    my_instance = instance_i;
    return 0;
  } else {
    BRAHMA_LOG_ERROR("%s instance_i is not set", "POSIX");
    throw std::runtime_error("instance_i is not set");
  }
}

int POSIX::open(const char *pathname, int flags, ...) {
  if (flags & O_CREAT) {
    va_list args;
    va_start(args, flags);
    int mode = va_arg(args, int);
    va_end(args);
    BRAHMA_UNWRAPPED_FUNC(open, int, (pathname, flags, mode));
    return result;
  } else {
    BRAHMA_UNWRAPPED_FUNC(open, int, (pathname, flags));
    return result;
  }
}

int POSIX::creat(const char *path, mode_t mode) {
  BRAHMA_UNWRAPPED_FUNC(creat, int, (path, mode));
  return result;
}

int POSIX::creat64(const char *path, mode_t mode) {
  BRAHMA_UNWRAPPED_FUNC(creat64, int, (path, mode));
  return result;
}

int POSIX::open64(const char *path, int flags, ...) {
  if (flags & O_CREAT) {
    va_list args;
    va_start(args, flags);
    int mode = va_arg(args, int);
    va_end(args);
    BRAHMA_UNWRAPPED_FUNC(open64, int, (path, flags, mode));
    return result;
  } else {
    BRAHMA_UNWRAPPED_FUNC(open64, int, (path, flags));
    return result;
  }
}

int POSIX::close(int fd) {
  BRAHMA_UNWRAPPED_FUNC(close, int, (fd));
  return result;
}

ssize_t POSIX::write(int fd, const void *buf, size_t count) {
  BRAHMA_UNWRAPPED_FUNC(write, ssize_t, (fd, buf, count));
  return result;
}

ssize_t POSIX::read(int fd, void *buf, size_t count) {
  BRAHMA_UNWRAPPED_FUNC(read, ssize_t, (fd, buf, count));
  return result;
}

off_t POSIX::lseek(int fd, off_t offset, int whence) {
  BRAHMA_UNWRAPPED_FUNC(lseek, off_t, (fd, offset, whence));
  return result;
}

off64_t POSIX::lseek64(int fd, off64_t offset, int whence) {
  BRAHMA_UNWRAPPED_FUNC(lseek, off64_t, (fd, offset, whence));
  return result;
}

ssize_t POSIX::pread(int fd, void *buf, size_t count, off_t offset) {
  BRAHMA_UNWRAPPED_FUNC(pread, ssize_t, (fd, buf, count, offset));
  return result;
}

ssize_t POSIX::pread64(int fd, void *buf, size_t count, off64_t offset) {
  BRAHMA_UNWRAPPED_FUNC(pread64, ssize_t, (fd, buf, count, offset));
  return result;
}

ssize_t POSIX::pwrite(int fd, const void *buf, size_t count, off64_t offset) {
  BRAHMA_UNWRAPPED_FUNC(pwrite, ssize_t, (fd, buf, count, offset));
  return result;
}

ssize_t POSIX::pwrite64(int fd, const void *buf, size_t count, off64_t offset) {
  BRAHMA_UNWRAPPED_FUNC(pwrite64, ssize_t, (fd, buf, count, offset));
  return result;
}

int POSIX::fsync(int fd) {
  BRAHMA_UNWRAPPED_FUNC(fsync, int, (fd));
  return result;
}

int POSIX::fdatasync(int fd) {
  BRAHMA_UNWRAPPED_FUNC(fdatasync, int, (fd));
  return result;
}

int POSIX::openat(int dirfd, const char *pathname, int flags, ...) {
  if (flags & O_CREAT) {
    va_list args;
    va_start(args, flags);
    int mode = va_arg(args, int);
    va_end(args);
    BRAHMA_UNWRAPPED_FUNC(openat, int, (dirfd, pathname, flags, mode));
    return result;
  } else {
    BRAHMA_UNWRAPPED_FUNC(openat, int, (dirfd, pathname, flags));
    return result;
  }
}
int POSIX::openat64(int dirfd, const char *pathname, int flags, ...) {
  if (flags & O_CREAT) {
    va_list args;
    va_start(args, flags);
    int mode = va_arg(args, int);
    va_end(args);
    BRAHMA_UNWRAPPED_FUNC(openat64, int, (dirfd, pathname, flags, mode));
    return result;
  } else {
    BRAHMA_UNWRAPPED_FUNC(openat64, int, (dirfd, pathname, flags));
    return result;
  }
}
int POSIX::__xstat(int vers, const char *path, struct stat *buf) {
  BRAHMA_UNWRAPPED_FUNC(__xstat, int, (vers, path, buf));
  return result;
}
int POSIX::__xstat64(int vers, const char *path, struct stat64 *buf) {
  BRAHMA_UNWRAPPED_FUNC(__xstat64, int, (vers, path, buf));
  return result;
}
int POSIX::__lxstat(int vers, const char *path, struct stat *buf) {
  BRAHMA_UNWRAPPED_FUNC(__lxstat, int, (vers, path, buf));
  return result;
}
int POSIX::__lxstat64(int vers, const char *path, struct stat64 *buf) {
  BRAHMA_UNWRAPPED_FUNC(__lxstat64, int, (vers, path, buf));
  return result;
}
int POSIX::__fxstat(int vers, int fd, struct stat *buf) {
  BRAHMA_UNWRAPPED_FUNC(__fxstat, int, (vers, fd, buf));
  return result;
}
int POSIX::__fxstat64(int vers, int fd, struct stat64 *buf) {
  BRAHMA_UNWRAPPED_FUNC(__fxstat64, int, (vers, fd, buf));
  return result;
}
int POSIX::__fxstatat(int vers, int dirfd, const char *path, struct stat *buf,
                      int flags) {
  BRAHMA_UNWRAPPED_FUNC(__fxstatat, int, (vers, dirfd, path, buf, flags));
  return result;
}
int POSIX::__fxstatat64(int vers, int dirfd, const char *path,
                        struct stat64 *buf, int flags) {
  BRAHMA_UNWRAPPED_FUNC(__fxstatat64, int, (vers, dirfd, path, buf, flags));
  return result;
}
int POSIX::__xmknod(int vers, const char *path, mode_t mode, dev_t *dev) {
  BRAHMA_UNWRAPPED_FUNC(__xmknod, int, (vers, path, mode, dev));
  return result;
}
int POSIX::__open_2(const char *path, int oflag) {
  BRAHMA_UNWRAPPED_FUNC(__open_2, int, (path, oflag));
  return result;
}
int POSIX::__open64_2(const char *path, int oflag) {
  BRAHMA_UNWRAPPED_FUNC(__open64_2, int, (path, oflag));
  return result;
}
int POSIX::__openat_2(int dirfd, const char *path, int oflag) {
  BRAHMA_UNWRAPPED_FUNC(__openat_2, int, (dirfd, path, oflag));
  return result;
}
int POSIX::__openat64_2(int dirfd, const char *path, int oflag) {
  BRAHMA_UNWRAPPED_FUNC(__openat64_2, int, (dirfd, path, oflag));
  return result;
}
ssize_t POSIX::__read_chk(int fd, void *buf, size_t nbytes, size_t buflen) {
  BRAHMA_UNWRAPPED_FUNC(__read_chk, ssize_t, (fd, buf, nbytes, buflen));
  return result;
}
ssize_t POSIX::__pread_chk(int fd, void *buf, size_t nbytes, off_t offset, size_t buflen) {
  BRAHMA_UNWRAPPED_FUNC(__pread_chk, ssize_t, (fd, buf, nbytes, offset, buflen));
  return result;
}
ssize_t POSIX::__pread64_chk(int fd, void *buf, size_t nbytes, off64_t offset, size_t buflen) {
  BRAHMA_UNWRAPPED_FUNC(__pread64_chk, ssize_t, (fd, buf, nbytes, offset, buflen));
  return result;
}
ssize_t POSIX::__readlink_chk(const char *path, char *buf, size_t len, size_t buflen) {
  BRAHMA_UNWRAPPED_FUNC(__readlink_chk, ssize_t, (path, buf, len, buflen));
  return result;
}
ssize_t POSIX::__readlinkat_chk(int dirfd, const char *path, char *buf, size_t len, size_t buflen) {
  BRAHMA_UNWRAPPED_FUNC(__readlinkat_chk, ssize_t, (dirfd, path, buf, len, buflen));
  return result;
}
char *POSIX::__getcwd_chk(char *buf, size_t size, size_t buflen) {
  BRAHMA_UNWRAPPED_FUNC(__getcwd_chk, char *, (buf, size, buflen));
  return result;
}
char *POSIX::__realpath_chk(const char *path, char *resolved, size_t resolvedlen) {
  BRAHMA_UNWRAPPED_FUNC(__realpath_chk, char *, (path, resolved, resolvedlen));
  return result;
}
char *POSIX::getcwd(char *buf, size_t size) {
  BRAHMA_UNWRAPPED_FUNC(getcwd, char *, (buf, size));
  return result;
}
int POSIX::mkdir(const char *pathname, mode_t mode) {
  BRAHMA_UNWRAPPED_FUNC(mkdir, int, (pathname, mode));
  return result;
}
int POSIX::rmdir(const char *pathname) {
  BRAHMA_UNWRAPPED_FUNC(rmdir, int, (pathname));
  return result;
}
int POSIX::chdir(const char *path) {
  BRAHMA_UNWRAPPED_FUNC(chdir, int, (path));
  return result;
}
int POSIX::link(const char *oldpath, const char *newpath) {
  BRAHMA_UNWRAPPED_FUNC(link, int, (oldpath, newpath));
  return result;
}
int POSIX::linkat(int fd1, const char *path1, int fd2, const char *path2,
                  int flag) {
  BRAHMA_UNWRAPPED_FUNC(linkat, int, (fd1, path1, fd2, path2, flag));
  return result;
}
int POSIX::unlink(const char *pathname) {
  BRAHMA_UNWRAPPED_FUNC(unlink, int, (pathname));
  return result;
}
int POSIX::symlink(const char *path1, const char *path2) {
  BRAHMA_UNWRAPPED_FUNC(symlink, int, (path1, path2));
  return result;
}
int POSIX::symlinkat(const char *path1, int fd, const char *path2) {
  BRAHMA_UNWRAPPED_FUNC(symlinkat, int, (path1, fd, path2));
  return result;
}
ssize_t POSIX::readlink(const char *path, char *buf, size_t bufsize) {
  BRAHMA_UNWRAPPED_FUNC(readlink, ssize_t, (path, buf, bufsize));
  return result;
}
ssize_t POSIX::readlinkat(int fd, const char *path, char *buf, size_t bufsize) {
  BRAHMA_UNWRAPPED_FUNC(readlinkat, ssize_t, (fd, path, buf, bufsize));
  return result;
}
int POSIX::rename(const char *oldpath, const char *newpath) {
  BRAHMA_UNWRAPPED_FUNC(rename, int, (oldpath, newpath));
  return result;
}
int POSIX::chmod(const char *path, mode_t mode) {
  BRAHMA_UNWRAPPED_FUNC(chmod, int, (path, mode));
  return result;
}
int POSIX::chown(const char *path, uid_t owner, gid_t group) {
  BRAHMA_UNWRAPPED_FUNC(chown, int, (path, owner, group));
  return result;
}
int POSIX::lchown(const char *path, uid_t owner, gid_t group) {
  BRAHMA_UNWRAPPED_FUNC(lchown, int, (path, owner, group));
  return result;
}
int POSIX::utime(const char *filename, const utimbuf *buf) {
  BRAHMA_UNWRAPPED_FUNC(utime, int, (filename, buf));
  return result;
}
DIR *POSIX::opendir(const char *name) {
  BRAHMA_UNWRAPPED_FUNC(opendir, DIR *, (name));
  return result;
}
dirent *POSIX::readdir(DIR *dir) {
  BRAHMA_UNWRAPPED_FUNC(readdir, dirent *, (dir));
  return result;
}
dirent64 *POSIX::readdir64(DIR *dir) {
  BRAHMA_UNWRAPPED_FUNC(readdir64, dirent64 *, (dir));
  return result;
}
int POSIX::closedir(DIR *dir) {
  BRAHMA_UNWRAPPED_FUNC(closedir, int, (dir));
  return result;
}
void POSIX::rewinddir(DIR *dir) {
  BRAHMA_UNWRAPPED_FUNC_VOID(rewinddir, (dir));
}
int POSIX::fcntl(int fd, int cmd, ...) {
  if (cmd == F_DUPFD || cmd == F_DUPFD_CLOEXEC || cmd == F_SETFD ||
      cmd == F_SETFL || cmd == F_SETOWN) {  // arg: int
    va_list arg;
    va_start(arg, cmd);
    int val = va_arg(arg, int);
    va_end(arg);
    BRAHMA_UNWRAPPED_FUNC(fcntl, int, (fd, cmd, val));
    return result;
  } else if (cmd == F_GETFD || cmd == F_GETFL || cmd == F_GETOWN) {
    BRAHMA_UNWRAPPED_FUNC(fcntl, int, (fd, cmd));
    return result;
  } else if (cmd == F_SETLK || cmd == F_SETLKW || cmd == F_GETLK) {
    va_list arg;
    va_start(arg, cmd);
    struct flock *lk = va_arg(arg, struct flock *);
    va_end(arg);
    BRAHMA_UNWRAPPED_FUNC(fcntl, int, (fd, cmd, lk));
    return result;
  } else {  // assume arg: void, cmd==F_GETOWN_EX || cmd==F_SETOWN_EX
            // ||cmd==F_GETSIG || cmd==F_SETSIG)
    BRAHMA_UNWRAPPED_FUNC(fcntl, int, (fd, cmd));
    return result;
  }
}
int POSIX::fcntl64(int fd, int cmd, ...) {
  if (cmd == F_DUPFD || cmd == F_DUPFD_CLOEXEC || cmd == F_SETFD ||
      cmd == F_SETFL || cmd == F_SETOWN) {  // arg: int
    va_list arg;
    va_start(arg, cmd);
    int val = va_arg(arg, int);
    va_end(arg);
    BRAHMA_UNWRAPPED_FUNC(fcntl64, int, (fd, cmd, val));
    return result;
  } else if (cmd == F_GETFD || cmd == F_GETFL || cmd == F_GETOWN) {
    BRAHMA_UNWRAPPED_FUNC(fcntl64, int, (fd, cmd));
    return result;
  } else if (cmd == F_SETLK || cmd == F_SETLKW || cmd == F_GETLK) {
    va_list arg;
    va_start(arg, cmd);
    struct flock *lk = va_arg(arg, struct flock *);
    va_end(arg);
    BRAHMA_UNWRAPPED_FUNC(fcntl64, int, (fd, cmd, lk));
    return result;
  } else {  // assume arg: void, cmd==F_GETOWN_EX || cmd==F_SETOWN_EX
            // ||cmd==F_GETSIG || cmd==F_SETSIG)
    BRAHMA_UNWRAPPED_FUNC(fcntl64, int, (fd, cmd));
    return result;
  }
}
int POSIX::dup(int oldfd) {
  BRAHMA_UNWRAPPED_FUNC(dup, int, (oldfd));
  return result;
}
int POSIX::dup2(int oldfd, int newfd) {
  BRAHMA_UNWRAPPED_FUNC(dup2, int, (oldfd, newfd));
  return result;
}
int POSIX::dup3(int oldfd, int newfd, int flags) {
  BRAHMA_UNWRAPPED_FUNC(dup3, int, (oldfd, newfd, flags));
  return result;
}
int POSIX::close_range(unsigned int first, unsigned int last, int flags) {
  BRAHMA_UNWRAPPED_FUNC(close_range, int, (first, last, flags));
  return result;
}
void POSIX::closefrom(int lowfd) {
  BRAHMA_UNWRAPPED_FUNC_VOID(closefrom, (lowfd));
}
int POSIX::pipe(int pipefd[2]) {
  BRAHMA_UNWRAPPED_FUNC(pipe, int, (pipefd));
  return result;
}
int POSIX::mkfifo(const char *pathname, mode_t mode) {
  BRAHMA_UNWRAPPED_FUNC(mkfifo, int, (pathname, mode));
  return result;
}
mode_t POSIX::umask(mode_t mask) {
  BRAHMA_UNWRAPPED_FUNC(umask, mode_t, (mask));
  return result;
}

int POSIX::access(const char *path, int amode) {
  BRAHMA_UNWRAPPED_FUNC(access, int, (path, amode));
  return result;
}
int POSIX::faccessat(int fd, const char *path, int amode, int flag) {
  BRAHMA_UNWRAPPED_FUNC(faccessat, int, (fd, path, amode, flag));
  return result;
}

int POSIX::remove(const char *pathname) {
  BRAHMA_UNWRAPPED_FUNC(remove, int, (pathname));
  return result;
}
int POSIX::truncate(const char *pathname, off_t length) {
  BRAHMA_UNWRAPPED_FUNC(truncate, int, (pathname, length));
  return result;
}

int POSIX::truncate64(const char *pathname, off64_t length) {
  BRAHMA_UNWRAPPED_FUNC(truncate64, int, (pathname, length));
  return result;
}

int POSIX::ftruncate(int fd, off_t length) {
  BRAHMA_UNWRAPPED_FUNC(ftruncate, int, (fd, length));
  return result;
}
int POSIX::ftruncate64(int fd, off64_t length) {
  BRAHMA_UNWRAPPED_FUNC(ftruncate64, int, (fd, length));
  return result;
}
int POSIX::execl(const char *pathname, const char *arg, ...) {
  va_list args;
  va_start(args, arg);
  BRAHMA_UNWRAPPED_FUNC(execl, int, (pathname, arg, args));
  va_end(args);
  return result;
}
int POSIX::execlp(const char *pathname, const char *arg, ...) {
  va_list args;
  va_start(args, arg);
  BRAHMA_UNWRAPPED_FUNC(execlp, int, (pathname, arg, args));
  va_end(args);
  return result;
}
int POSIX::execv(const char *pathname, char *const argv[]) {
  BRAHMA_UNWRAPPED_FUNC(execv, int, (pathname, argv));
  return result;
}
int POSIX::execvp(const char *pathname, char *const argv[]) {
  BRAHMA_UNWRAPPED_FUNC(execvp, int, (pathname, argv));
  return result;
}
int POSIX::execvpe(const char *pathname, char *const argv[],
                   char *const envp[]) {
  BRAHMA_UNWRAPPED_FUNC(execvpe, int, (pathname, argv, envp));
  return result;
}
int POSIX::fork() {
  BRAHMA_UNWRAPPED_FUNC(fork, int, ());
  return result;
}
void POSIX::exit(int status) {
  BRAHMA_UNWRAPPED_FUNC_VOID(exit, (status));
}
void POSIX::_exit(int status) {
  BRAHMA_UNWRAPPED_FUNC_VOID(_exit, (status));
}
void *POSIX::mmap(void *addr, size_t length, int prot, int flags, int fd,
                  off_t offset) {
  BRAHMA_UNWRAPPED_FUNC(mmap, void *, (addr, length, prot, flags, fd, offset));
  return result;
}
void *POSIX::mmap64(void *addr, size_t length, int prot, int flags, int fd,
                    off64_t offset) {
  BRAHMA_UNWRAPPED_FUNC(mmap64, void *,
                        (addr, length, prot, flags, fd, offset));
  return result;
}
int POSIX::munmap(void *addr, size_t len) {
  BRAHMA_UNWRAPPED_FUNC(munmap, int,
                        (addr, len));
  return result;
}
int POSIX::msync(void *addr, size_t len, int flags) {
  BRAHMA_UNWRAPPED_FUNC(msync, int,
                        (addr, len,flags));
  return result;
}
long POSIX::sysconf(int name) {
  BRAHMA_UNWRAPPED_FUNC(sysconf, long,
                        (name));
  return result;
}
int POSIX::madvise(void *addr, size_t length, int advice) {
  BRAHMA_UNWRAPPED_FUNC(madvise, int,
                        (addr, length, advice));
  return result;
}
int POSIX::mprotect(void *addr, size_t len, int prot) {
  BRAHMA_UNWRAPPED_FUNC(mprotect, int,
                        (addr, len, prot));
  return result;
}
int POSIX::mlock(const void *addr, size_t len) {
  BRAHMA_UNWRAPPED_FUNC(mlock, int,
                        (addr, len));
  return result;
}
int POSIX::munlock(const void *addr, size_t len) {
  BRAHMA_UNWRAPPED_FUNC(munlock, int,
                        (addr, len));
  return result;
}
int POSIX::mlockall(int flags) {
  BRAHMA_UNWRAPPED_FUNC(mlockall, int,
                        (flags));
  return result;
}
int POSIX::munlockall() {
  BRAHMA_UNWRAPPED_FUNC(munlockall, int, ());
  return result;
}

void POSIX::_fini(void) {
}

int POSIX::stat(const char *path, struct stat *buf) {
  BRAHMA_UNWRAPPED_FUNC(stat, int, (path, buf));
  return result;
}
int POSIX::lstat(const char *path, struct stat *buf) {
  BRAHMA_UNWRAPPED_FUNC(lstat, int, (path, buf));
  return result;
}
int POSIX::fstat(int fd, struct stat *buf) {
  BRAHMA_UNWRAPPED_FUNC(fstat, int, (fd, buf));
  return result;
}
int POSIX::fstatat(int dirfd, const char *path, struct stat *buf,
                   int flags) {
  BRAHMA_UNWRAPPED_FUNC(fstatat, int, (dirfd, path, buf, flags));
  return result;
}
int POSIX::stat64(const char *path, struct stat64 *buf) {
  BRAHMA_UNWRAPPED_FUNC(stat64, int, (path, buf));
  return result;
}
int POSIX::lstat64(const char *path, struct stat64 *buf) {
  BRAHMA_UNWRAPPED_FUNC(lstat64, int, (path, buf));
  return result;
}
int POSIX::fstat64(int fd, struct stat64 *buf) {
  BRAHMA_UNWRAPPED_FUNC(fstat64, int, (fd, buf));
  return result;
}
int POSIX::fstatat64(int dirfd, const char *path, struct stat64 *buf,
                     int flags) {
  BRAHMA_UNWRAPPED_FUNC(fstatat64, int, (dirfd, path, buf, flags));
  return result;
}
int POSIX::posix_fadvise(int fd, off_t offset, off_t len, int advice) {
  BRAHMA_UNWRAPPED_FUNC(posix_fadvise, int, (fd, offset, len, advice));
  return result;
}
int POSIX::posix_fadvise64(int fd, off64_t offset, off64_t len, int advice) {
  BRAHMA_UNWRAPPED_FUNC(posix_fadvise64, int, (fd, offset, len, advice));
  return result;
}
int POSIX::posix_fallocate(int fd, off_t offset, off_t len) {
  BRAHMA_UNWRAPPED_FUNC(posix_fallocate, int, (fd, offset, len));
  return result;
}
int POSIX::posix_fallocate64(int fd, off64_t offset, off64_t len) {
  BRAHMA_UNWRAPPED_FUNC(posix_fallocate64, int, (fd, offset, len));
  return result;
}
int POSIX::flock(int fd, int operation) {
  BRAHMA_UNWRAPPED_FUNC(flock, int, (fd, operation));
  return result;
}
ssize_t POSIX::readv(int fd, const struct iovec *iov, int iovcnt) {
  BRAHMA_UNWRAPPED_FUNC(readv, ssize_t, (fd, iov, iovcnt));
  return result;
}
ssize_t POSIX::writev(int fd, const struct iovec *iov, int iovcnt) {
  BRAHMA_UNWRAPPED_FUNC(writev, ssize_t, (fd, iov, iovcnt));
  return result;
}
ssize_t POSIX::preadv(int fd, const struct iovec *iov, int iovcnt,
                      off_t offset) {
  BRAHMA_UNWRAPPED_FUNC(preadv, ssize_t, (fd, iov, iovcnt, offset));
  return result;
}
ssize_t POSIX::preadv64(int fd, const struct iovec *iov, int iovcnt,
                        off64_t offset) {
  BRAHMA_UNWRAPPED_FUNC(preadv64, ssize_t, (fd, iov, iovcnt, offset));
  return result;
}
ssize_t POSIX::pwritev(int fd, const struct iovec *iov, int iovcnt,
                       off_t offset) {
  BRAHMA_UNWRAPPED_FUNC(pwritev, ssize_t, (fd, iov, iovcnt, offset));
  return result;
}
ssize_t POSIX::pwritev64(int fd, const struct iovec *iov, int iovcnt,
                         off64_t offset) {
  BRAHMA_UNWRAPPED_FUNC(pwritev64, ssize_t, (fd, iov, iovcnt, offset));
  return result;
}
int POSIX::renameat(int olddirfd, const char *oldpath, int newdirfd,
                    const char *newpath) {
  BRAHMA_UNWRAPPED_FUNC(renameat, int,
                        (olddirfd, oldpath, newdirfd, newpath));
  return result;
}
int POSIX::mkdirat(int dirfd, const char *pathname, mode_t mode) {
  BRAHMA_UNWRAPPED_FUNC(mkdirat, int, (dirfd, pathname, mode));
  return result;
}
int POSIX::unlinkat(int dirfd, const char *pathname, int flags) {
  BRAHMA_UNWRAPPED_FUNC(unlinkat, int, (dirfd, pathname, flags));
  return result;
}
int POSIX::fchmodat(int dirfd, const char *pathname, mode_t mode,
                    int flags) {
  BRAHMA_UNWRAPPED_FUNC(fchmodat, int, (dirfd, pathname, mode, flags));
  return result;
}
int POSIX::fchownat(int dirfd, const char *pathname, uid_t owner,
                    gid_t group, int flags) {
  BRAHMA_UNWRAPPED_FUNC(fchownat, int, (dirfd, pathname, owner, group, flags));
  return result;
}
int POSIX::fchmod(int fd, mode_t mode) {
  BRAHMA_UNWRAPPED_FUNC(fchmod, int, (fd, mode));
  return result;
}
int POSIX::fchown(int fd, uid_t owner, gid_t group) {
  BRAHMA_UNWRAPPED_FUNC(fchown, int, (fd, owner, group));
  return result;
}
int POSIX::execve(const char *pathname, char *const argv[],
                  char *const envp[]) {
  BRAHMA_UNWRAPPED_FUNC(execve, int, (pathname, argv, envp));
  return result;
}
pid_t POSIX::waitpid(pid_t pid, int *wstatus, int options) {
  BRAHMA_UNWRAPPED_FUNC(waitpid, pid_t, (pid, wstatus, options));
  return result;
}
pid_t POSIX::wait(int *wstatus) {
  BRAHMA_UNWRAPPED_FUNC(wait, pid_t, (wstatus));
  return result;
}
char *POSIX::realpath(const char *path, char *resolved_path) {
  BRAHMA_UNWRAPPED_FUNC(realpath, char *, (path, resolved_path));
  return result;
}
int POSIX::dirfd(DIR *dir) {
  BRAHMA_UNWRAPPED_FUNC(dirfd, int, (dir));
  return result;
}
int POSIX::mknod(const char *pathname, mode_t mode, dev_t dev) {
  BRAHMA_UNWRAPPED_FUNC(mknod, int, (pathname, mode, dev));
  return result;
}
ssize_t POSIX::sendfile(int out_fd, int in_fd, off_t *offset, size_t count) {
  BRAHMA_UNWRAPPED_FUNC(sendfile, ssize_t, (out_fd, in_fd, offset, count));
  return result;
}
ssize_t POSIX::sendfile64(int out_fd, int in_fd, off64_t *offset,
                          size_t count) {
  BRAHMA_UNWRAPPED_FUNC(sendfile64, ssize_t, (out_fd, in_fd, offset, count));
  return result;
}
ssize_t POSIX::copy_file_range(int fd_in, off64_t *off_in, int fd_out,
                               off64_t *off_out, size_t len,
                               unsigned int flags) {
  BRAHMA_UNWRAPPED_FUNC(copy_file_range, ssize_t,
                        (fd_in, off_in, fd_out, off_out, len, flags));
  return result;
}
int POSIX::statvfs(const char *path, struct statvfs *buf) {
  BRAHMA_UNWRAPPED_FUNC(statvfs, int, (path, buf));
  return result;
}
int POSIX::statvfs64(const char *path, struct statvfs64 *buf) {
  BRAHMA_UNWRAPPED_FUNC(statvfs64, int, (path, buf));
  return result;
}
int POSIX::fstatvfs(int fd, struct statvfs *buf) {
  BRAHMA_UNWRAPPED_FUNC(fstatvfs, int, (fd, buf));
  return result;
}
int POSIX::fstatvfs64(int fd, struct statvfs64 *buf) {
  BRAHMA_UNWRAPPED_FUNC(fstatvfs64, int, (fd, buf));
  return result;
}
ssize_t POSIX::preadv2(int fd, const struct iovec *iov, int iovcnt, off_t offset, int flags) {
  BRAHMA_UNWRAPPED_FUNC(preadv2, ssize_t, (fd, iov, iovcnt, offset, flags));
  return result;
}
ssize_t POSIX::pwritev2(int fd, const struct iovec *iov, int iovcnt, off_t offset, int flags) {
  BRAHMA_UNWRAPPED_FUNC(pwritev2, ssize_t, (fd, iov, iovcnt, offset, flags));
  return result;
}
ssize_t POSIX::preadv64v2(int fd, const struct iovec *iov, int iovcnt, off64_t offset, int flags) {
  BRAHMA_UNWRAPPED_FUNC(preadv64v2, ssize_t, (fd, iov, iovcnt, offset, flags));
  return result;
}
ssize_t POSIX::pwritev64v2(int fd, const struct iovec *iov, int iovcnt, off64_t offset, int flags) {
  BRAHMA_UNWRAPPED_FUNC(pwritev64v2, ssize_t, (fd, iov, iovcnt, offset, flags));
  return result;
}
ssize_t POSIX::splice(int fd_in, off64_t *off_in, int fd_out, off64_t *off_out, size_t len, unsigned int flags) {
  BRAHMA_UNWRAPPED_FUNC(splice, ssize_t, (fd_in, off_in, fd_out, off_out, len, flags));
  return result;
}
ssize_t POSIX::tee(int fd_in, int fd_out, size_t len, unsigned int flags) {
  BRAHMA_UNWRAPPED_FUNC(tee, ssize_t, (fd_in, fd_out, len, flags));
  return result;
}
ssize_t POSIX::vmsplice(int fd, const struct iovec *iov, size_t nr_segs, unsigned int flags) {
  BRAHMA_UNWRAPPED_FUNC(vmsplice, ssize_t, (fd, iov, nr_segs, flags));
  return result;
}
int POSIX::fallocate(int fd, int mode, off_t offset, off_t len) {
  BRAHMA_UNWRAPPED_FUNC(fallocate, int, (fd, mode, offset, len));
  return result;
}
int POSIX::fallocate64(int fd, int mode, off64_t offset, off64_t len) {
  BRAHMA_UNWRAPPED_FUNC(fallocate64, int, (fd, mode, offset, len));
  return result;
}
ssize_t POSIX::readahead(int fd, off64_t offset, size_t count) {
  BRAHMA_UNWRAPPED_FUNC(readahead, ssize_t, (fd, offset, count));
  return result;
}
void POSIX::sync(void) {
  BRAHMA_UNWRAPPED_FUNC_VOID(sync, ());
}
int POSIX::syncfs(int fd) {
  BRAHMA_UNWRAPPED_FUNC(syncfs, int, (fd));
  return result;
}
int POSIX::sync_file_range(int fd, off64_t offset, off64_t nbytes, unsigned int flags) {
  BRAHMA_UNWRAPPED_FUNC(sync_file_range, int, (fd, offset, nbytes, flags));
  return result;
}
int POSIX::statx(int dirfd, const char *path, int flags, unsigned int mask, struct statx *buf) {
  BRAHMA_UNWRAPPED_FUNC(statx, int, (dirfd, path, flags, mask, buf));
  return result;
}
int POSIX::statfs(const char *path, struct statfs *buf) {
  BRAHMA_UNWRAPPED_FUNC(statfs, int, (path, buf));
  return result;
}
int POSIX::fstatfs(int fd, struct statfs *buf) {
  BRAHMA_UNWRAPPED_FUNC(fstatfs, int, (fd, buf));
  return result;
}
int POSIX::statfs64(const char *path, struct statfs64 *buf) {
  BRAHMA_UNWRAPPED_FUNC(statfs64, int, (path, buf));
  return result;
}
int POSIX::fstatfs64(int fd, struct statfs64 *buf) {
  BRAHMA_UNWRAPPED_FUNC(fstatfs64, int, (fd, buf));
  return result;
}
int POSIX::mknodat(int dirfd, const char *path, mode_t mode, dev_t dev) {
  BRAHMA_UNWRAPPED_FUNC(mknodat, int, (dirfd, path, mode, dev));
  return result;
}
int POSIX::__xmknodat(int ver, int dirfd, const char *path, mode_t mode, dev_t *dev) {
  BRAHMA_UNWRAPPED_FUNC(__xmknodat, int, (ver, dirfd, path, mode, dev));
  return result;
}
int POSIX::euidaccess(const char *path, int mode) {
  BRAHMA_UNWRAPPED_FUNC(euidaccess, int, (path, mode));
  return result;
}
int POSIX::eaccess(const char *path, int mode) {
  BRAHMA_UNWRAPPED_FUNC(eaccess, int, (path, mode));
  return result;
}
int POSIX::renameat2(int olddirfd, const char *oldpath, int newdirfd, const char *newpath, unsigned int flags) {
  BRAHMA_UNWRAPPED_FUNC(renameat2, int, (olddirfd, oldpath, newdirfd, newpath, flags));
  return result;
}
int POSIX::mkfifoat(int dirfd, const char *path, mode_t mode) {
  BRAHMA_UNWRAPPED_FUNC(mkfifoat, int, (dirfd, path, mode));
  return result;
}
int POSIX::lchmod(const char *path, mode_t mode) {
  BRAHMA_UNWRAPPED_FUNC(lchmod, int, (path, mode));
  return result;
}
int POSIX::utimes(const char *path, const struct timeval times[2]) {
  BRAHMA_UNWRAPPED_FUNC(utimes, int, (path, times));
  return result;
}
int POSIX::futimes(int fd, const struct timeval tv[2]) {
  BRAHMA_UNWRAPPED_FUNC(futimes, int, (fd, tv));
  return result;
}
int POSIX::lutimes(const char *path, const struct timeval tv[2]) {
  BRAHMA_UNWRAPPED_FUNC(lutimes, int, (path, tv));
  return result;
}
int POSIX::futimesat(int dirfd, const char *path, const struct timeval times[2]) {
  BRAHMA_UNWRAPPED_FUNC(futimesat, int, (dirfd, path, times));
  return result;
}
int POSIX::utimensat(int dirfd, const char *path, const struct timespec times[2], int flags) {
  BRAHMA_UNWRAPPED_FUNC(utimensat, int, (dirfd, path, times, flags));
  return result;
}
int POSIX::futimens(int fd, const struct timespec times[2]) {
  BRAHMA_UNWRAPPED_FUNC(futimens, int, (fd, times));
  return result;
}
int POSIX::fchdir(int fd) {
  BRAHMA_UNWRAPPED_FUNC(fchdir, int, (fd));
  return result;
}
DIR *POSIX::fdopendir(int fd) {
  BRAHMA_UNWRAPPED_FUNC(fdopendir, DIR *, (fd));
  return result;
}
int POSIX::readdir_r(DIR *dir, struct dirent *entry, struct dirent **res) {
  BRAHMA_UNWRAPPED_FUNC(readdir_r, int, (dir, entry, res));
  return result;
}
int POSIX::readdir64_r(DIR *dir, struct dirent64 *entry, struct dirent64 **res) {
  BRAHMA_UNWRAPPED_FUNC(readdir64_r, int, (dir, entry, res));
  return result;
}
void POSIX::seekdir(DIR *dir, long loc) {
  BRAHMA_UNWRAPPED_FUNC_VOID(seekdir, (dir, loc));
}
long POSIX::telldir(DIR *dir) {
  BRAHMA_UNWRAPPED_FUNC(telldir, long, (dir));
  return result;
}
int POSIX::scandir(const char *dir, struct dirent ***namelist, int (*filter)(const struct dirent *), int (*compar)(const struct dirent **, const struct dirent **)) {
  BRAHMA_UNWRAPPED_FUNC(scandir, int, (dir, namelist, filter, compar));
  return result;
}
int POSIX::scandir64(const char *dir, struct dirent64 ***namelist, int (*filter)(const struct dirent64 *), int (*compar)(const struct dirent64 **, const struct dirent64 **)) {
  BRAHMA_UNWRAPPED_FUNC(scandir64, int, (dir, namelist, filter, compar));
  return result;
}
int POSIX::scandirat(int dirfd, const char *dir, struct dirent ***namelist, int (*filter)(const struct dirent *), int (*compar)(const struct dirent **, const struct dirent **)) {
  BRAHMA_UNWRAPPED_FUNC(scandirat, int, (dirfd, dir, namelist, filter, compar));
  return result;
}
int POSIX::scandirat64(int dirfd, const char *dir, struct dirent64 ***namelist, int (*filter)(const struct dirent64 *), int (*compar)(const struct dirent64 **, const struct dirent64 **)) {
  BRAHMA_UNWRAPPED_FUNC(scandirat64, int, (dirfd, dir, namelist, filter, compar));
  return result;
}
int POSIX::nftw(const char *path, int (*func)(const char *, const struct stat *, int, struct FTW *), int nopenfd, int flags) {
  BRAHMA_UNWRAPPED_FUNC(nftw, int, (path, func, nopenfd, flags));
  return result;
}
int POSIX::nftw64(const char *path, int (*func)(const char *, const struct stat64 *, int, struct FTW *), int nopenfd, int flags) {
  BRAHMA_UNWRAPPED_FUNC(nftw64, int, (path, func, nopenfd, flags));
  return result;
}
ssize_t POSIX::getdents64(int fd, void *buf, size_t count) {
  BRAHMA_UNWRAPPED_FUNC(getdents64, ssize_t, (fd, buf, count));
  return result;
}
void *POSIX::mremap(void *old_addr, size_t old_size, size_t new_size, int flags,
                   void *new_addr) {
  if (flags & MREMAP_FIXED) {
    BRAHMA_UNWRAPPED_FUNC(mremap, void *,
                          (old_addr, old_size, new_size, flags, new_addr));
    return result;
  } else {
    BRAHMA_UNWRAPPED_FUNC(mremap, void *,
                          (old_addr, old_size, new_size, flags));
    return result;
  }
}
int POSIX::ioctl(int fd, unsigned long request, void *arg) {
  BRAHMA_UNWRAPPED_FUNC(ioctl, int, (fd, request, arg));
  return result;
}
int POSIX::lockf(int fd, int cmd, off_t len) {
  BRAHMA_UNWRAPPED_FUNC(lockf, int, (fd, cmd, len));
  return result;
}
int POSIX::lockf64(int fd, int cmd, off64_t len) {
  BRAHMA_UNWRAPPED_FUNC(lockf64, int, (fd, cmd, len));
  return result;
}
int POSIX::pipe2(int pipefd[2], int flags) {
  BRAHMA_UNWRAPPED_FUNC(pipe2, int, (pipefd, flags));
  return result;
}
int POSIX::memfd_create(const char *name, unsigned int flags) {
  BRAHMA_UNWRAPPED_FUNC(memfd_create, int, (name, flags));
  return result;
}
int POSIX::mkstemp(char *tmpl) {
  BRAHMA_UNWRAPPED_FUNC(mkstemp, int, (tmpl));
  return result;
}
int POSIX::mkstemp64(char *tmpl) {
  BRAHMA_UNWRAPPED_FUNC(mkstemp64, int, (tmpl));
  return result;
}
int POSIX::mkostemp(char *tmpl, int flags) {
  BRAHMA_UNWRAPPED_FUNC(mkostemp, int, (tmpl, flags));
  return result;
}
int POSIX::mkostemp64(char *tmpl, int flags) {
  BRAHMA_UNWRAPPED_FUNC(mkostemp64, int, (tmpl, flags));
  return result;
}
int POSIX::mkstemps(char *tmpl, int suffixlen) {
  BRAHMA_UNWRAPPED_FUNC(mkstemps, int, (tmpl, suffixlen));
  return result;
}
int POSIX::mkstemps64(char *tmpl, int suffixlen) {
  BRAHMA_UNWRAPPED_FUNC(mkstemps64, int, (tmpl, suffixlen));
  return result;
}
int POSIX::mkostemps(char *tmpl, int suffixlen, int flags) {
  BRAHMA_UNWRAPPED_FUNC(mkostemps, int, (tmpl, suffixlen, flags));
  return result;
}
int POSIX::mkostemps64(char *tmpl, int suffixlen, int flags) {
  BRAHMA_UNWRAPPED_FUNC(mkostemps64, int, (tmpl, suffixlen, flags));
  return result;
}
char *POSIX::mkdtemp(char *tmpl) {
  BRAHMA_UNWRAPPED_FUNC(mkdtemp, char *, (tmpl));
  return result;
}
ssize_t POSIX::getxattr(const char *path, const char *name, void *value, size_t size) {
  BRAHMA_UNWRAPPED_FUNC(getxattr, ssize_t, (path, name, value, size));
  return result;
}
ssize_t POSIX::lgetxattr(const char *path, const char *name, void *value, size_t size) {
  BRAHMA_UNWRAPPED_FUNC(lgetxattr, ssize_t, (path, name, value, size));
  return result;
}
ssize_t POSIX::fgetxattr(int fd, const char *name, void *value, size_t size) {
  BRAHMA_UNWRAPPED_FUNC(fgetxattr, ssize_t, (fd, name, value, size));
  return result;
}
int POSIX::setxattr(const char *path, const char *name, const void *value, size_t size, int flags) {
  BRAHMA_UNWRAPPED_FUNC(setxattr, int, (path, name, value, size, flags));
  return result;
}
int POSIX::lsetxattr(const char *path, const char *name, const void *value, size_t size, int flags) {
  BRAHMA_UNWRAPPED_FUNC(lsetxattr, int, (path, name, value, size, flags));
  return result;
}
int POSIX::fsetxattr(int fd, const char *name, const void *value, size_t size, int flags) {
  BRAHMA_UNWRAPPED_FUNC(fsetxattr, int, (fd, name, value, size, flags));
  return result;
}
ssize_t POSIX::listxattr(const char *path, char *list, size_t size) {
  BRAHMA_UNWRAPPED_FUNC(listxattr, ssize_t, (path, list, size));
  return result;
}
ssize_t POSIX::llistxattr(const char *path, char *list, size_t size) {
  BRAHMA_UNWRAPPED_FUNC(llistxattr, ssize_t, (path, list, size));
  return result;
}
ssize_t POSIX::flistxattr(int fd, char *list, size_t size) {
  BRAHMA_UNWRAPPED_FUNC(flistxattr, ssize_t, (fd, list, size));
  return result;
}
int POSIX::removexattr(const char *path, const char *name) {
  BRAHMA_UNWRAPPED_FUNC(removexattr, int, (path, name));
  return result;
}
int POSIX::lremovexattr(const char *path, const char *name) {
  BRAHMA_UNWRAPPED_FUNC(lremovexattr, int, (path, name));
  return result;
}
int POSIX::fremovexattr(int fd, const char *name) {
  BRAHMA_UNWRAPPED_FUNC(fremovexattr, int, (fd, name));
  return result;
}
int POSIX::aio_read(struct aiocb *cb) {
  BRAHMA_UNWRAPPED_FUNC(aio_read, int, (cb));
  return result;
}
int POSIX::aio_write(struct aiocb *cb) {
  BRAHMA_UNWRAPPED_FUNC(aio_write, int, (cb));
  return result;
}
int POSIX::aio_error(const struct aiocb *cb) {
  BRAHMA_UNWRAPPED_FUNC(aio_error, int, (cb));
  return result;
}
ssize_t POSIX::aio_return(struct aiocb *cb) {
  BRAHMA_UNWRAPPED_FUNC(aio_return, ssize_t, (cb));
  return result;
}
int POSIX::aio_fsync(int op, struct aiocb *cb) {
  BRAHMA_UNWRAPPED_FUNC(aio_fsync, int, (op, cb));
  return result;
}
int POSIX::aio_cancel(int fd, struct aiocb *cb) {
  BRAHMA_UNWRAPPED_FUNC(aio_cancel, int, (fd, cb));
  return result;
}
int POSIX::aio_suspend(const struct aiocb *const list[], int nent, const struct timespec *timeout) {
  BRAHMA_UNWRAPPED_FUNC(aio_suspend, int, (list, nent, timeout));
  return result;
}
int POSIX::lio_listio(int mode, struct aiocb *const list[], int nent, struct sigevent *sig) {
  BRAHMA_UNWRAPPED_FUNC(lio_listio, int, (mode, list, nent, sig));
  return result;
}
int POSIX::aio_read64(struct aiocb64 *cb) {
  BRAHMA_UNWRAPPED_FUNC(aio_read64, int, (cb));
  return result;
}
int POSIX::aio_write64(struct aiocb64 *cb) {
  BRAHMA_UNWRAPPED_FUNC(aio_write64, int, (cb));
  return result;
}
int POSIX::aio_error64(const struct aiocb64 *cb) {
  BRAHMA_UNWRAPPED_FUNC(aio_error64, int, (cb));
  return result;
}
ssize_t POSIX::aio_return64(struct aiocb64 *cb) {
  BRAHMA_UNWRAPPED_FUNC(aio_return64, ssize_t, (cb));
  return result;
}
int POSIX::aio_fsync64(int op, struct aiocb64 *cb) {
  BRAHMA_UNWRAPPED_FUNC(aio_fsync64, int, (op, cb));
  return result;
}
int POSIX::aio_cancel64(int fd, struct aiocb64 *cb) {
  BRAHMA_UNWRAPPED_FUNC(aio_cancel64, int, (fd, cb));
  return result;
}
int POSIX::aio_suspend64(const struct aiocb64 *const list[], int nent, const struct timespec *timeout) {
  BRAHMA_UNWRAPPED_FUNC(aio_suspend64, int, (list, nent, timeout));
  return result;
}
int POSIX::lio_listio64(int mode, struct aiocb64 *const list[], int nent, struct sigevent *sig) {
  BRAHMA_UNWRAPPED_FUNC(lio_listio64, int, (mode, list, nent, sig));
  return result;
}


// Set the unbindings - call the original function with a higher priority
size_t brahma::POSIX::unbind() {
  num_bindings = unbindings.size();
  if (num_bindings > 0) {
    gotcha_binding_t *raw_bindings = unbindings.data();
    char unbind_name[128];
    // See POSIX::bind(): sprintf is itself an interceptable STDIO function,
    // so it can't safely be used for this internal bookkeeping.
    strcpy(unbind_name, tool_name);
    strcat(unbind_name, "_unbind");
    gotcha_wrap(raw_bindings, num_bindings, unbind_name);
    gotcha_set_priority(unbind_name, bind_priority+1);
  }
  return num_bindings;
}

}  // namespace brahma