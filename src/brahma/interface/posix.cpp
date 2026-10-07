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