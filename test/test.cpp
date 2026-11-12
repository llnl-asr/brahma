//
// Created by hariharan on 8/8/22.
//
/* Brahma Header */
#include <brahma/brahma.h>
/* External Headers*/
#include <assert.h>
#include <brahma/brahma.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <utime.h>

#ifdef NDEBUG
#error "Assertions are disabled! Remove NDEBUG to enable them."
#endif

#ifdef BRAHMA_ENABLE_MPI
#include <mpi.h>

#define LEN 10

#define CHECK_ERR(func)                                              \
  {                                                                  \
    if (err != MPI_SUCCESS) {                                        \
      int errorStringLen;                                            \
      char errorString[MPI_MAX_ERROR_STRING];                        \
      MPI_Error_string(err, errorString, &errorStringLen);           \
      printf("Error at line %d: calling %s (%s)\n", __LINE__, #func, \
             errorString);                                           \
    }                                                                \
  }
#endif
namespace brahma {
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
    printf("1 Captured open call\n");
    api_count++;
    return 0;
  }

  int creat64(const char *path, mode_t mode) override {
    printf("2 Captured creat64 call\n");
    api_count++;
    return 0;
  }

  int open64(const char *path, int flags, ...) override {
    printf("3 Captured open64 call\n");
    api_count++;
    return 0;
  }

  int close(int fd) override {
    printf("4 Captured close call\n");
    api_count++;
    return 0;
  }

  ssize_t write(int fd, const void *buf, size_t count) override {
    printf("5 Captured write call\n");
    api_count++;
    return 0;
  }

  ssize_t read(int fd, void *buf, size_t count) override {
    printf("6 Captured read call\n");
    api_count++;
    return 0;
  }

  off_t lseek(int fd, off_t offset, int whence) override {
    printf("7 Captured lseek call\n");
    api_count++;
    return 0;
  }

  off64_t lseek64(int fd, off64_t offset, int whence) override {
    printf("8 Captured lseek64 call\n");
    api_count++;
    return 0;
  }

  ssize_t pread(int fd, void *buf, size_t count, off_t offset) override {
    printf("9 Captured pread call\n");
    api_count++;
    return 0;
  }

  ssize_t pread64(int fd, void *buf, size_t count, off64_t offset) override {
    printf("10 Captured pread64 call\n");
    api_count++;
    return 0;
  }

  ssize_t pwrite(int fd, const void *buf, size_t count,
                 off64_t offset) override {
    printf("11 Captured pwrite call\n");
    api_count++;
    return 0;
  }

  ssize_t pwrite64(int fd, const void *buf, size_t count,
                   off64_t offset) override {
    printf("12 Captured pwrite64 call\n");
    api_count++;
    return 0;
  }

  int fsync(int fd) override {
    printf("13 Captured fsync call\n");
    api_count++;
    return 0;
  }

  int fdatasync(int fd) override {
    printf("14 Captured fdatasync call\n");
    api_count++;
    return 0;
  }

  int openat(int dirfd, const char *pathname, int flags, ...) override {
    printf("15 Captured openat call\n");
    api_count++;
    return 0;
  }

  char *getcwd(char *buf, size_t size) override {
    printf("16 Captured getcwd call\n");
    api_count++;
    return 0;
  }

  int mkdir(const char *pathname, mode_t mode) override {
    printf("17 Captured mkdir call\n");
    api_count++;
    return 0;
  }

  int rmdir(const char *pathname) override {
    printf("18 Captured rmdir call\n");
    api_count++;
    return 0;
  }

  int chdir(const char *path) override {
    printf("19 Captured chdir call\n");
    api_count++;
    return 0;
  }

  int link(const char *oldpath, const char *newpath) override {
    printf("20 Captured link call\n");
    api_count++;
    return 0;
  }

  int linkat(int fd1, const char *path1, int fd2, const char *path2,
             int flag) override {
    printf("21 Captured linkat call\n");
    api_count++;
    return 0;
  }

  int unlink(const char *pathname) override {
    printf("22 Captured unlink call\n");
    api_count++;
    return 0;
  }

  int symlink(const char *path1, const char *path2) override {
    printf("23 Captured symlink call\n");
    api_count++;
    return 0;
  }

  int symlinkat(const char *path1, int fd, const char *path2) override {
    printf("24 Captured symlinkat call\n");
    api_count++;
    return 0;
  }

  ssize_t readlink(const char *path, char *buf, size_t bufsize) override {
    printf("25 Captured readlink call\n");
    api_count++;
    return 0;
  }
  ssize_t readlinkat(int fd, const char *path, char *buf,
                     size_t bufsize) override {
    printf("26 Captured readlinkat call\n");
    api_count++;
    return 0;
  }

  int rename(const char *oldpath, const char *newpath) override {
    printf("27 Captured rename call\n");
    api_count++;
    return 0;
  }

  int chmod(const char *path, mode_t mode) override {
    printf("28 Captured chmod call\n");
    api_count++;
    return 0;
  }

  int chown(const char *path, uid_t owner, gid_t group) override {
    printf("29 Captured chown call\n");
    api_count++;
    return 0;
  }

  int lchown(const char *path, uid_t owner, gid_t group) override {
    printf("30 Captured lchown call\n");
    api_count++;
    return 0;
  }

  int utime(const char *filename, const utimbuf *buf) override {
    printf("31 Captured utime call\n");
    api_count++;
    return 0;
  }

  DIR *opendir(const char *name) override {
    printf("32 Captured opendir call\n");
    api_count++;
    return 0;
  }

  dirent *readdir(DIR *dir) override {
    printf("33 Captured readdir call\n");
    api_count++;
    return 0;
  }
  int closedir(DIR *dir) override {
    printf("34 Captured closedir call\n");
    api_count++;
    return 0;
  }

  void rewinddir(DIR *dir) override {
    printf("35 Captured rewinddir call\n");
    api_count++;
  }

  int fcntl(int fd, int cmd, ...) override {
    printf("36 Captured fcntl call\n");
    api_count++;
    return 0;
  }

  int dup(int oldfd) override {
    printf("37 Captured dup call\n");
    api_count++;
    return 0;
  }

  int dup2(int oldfd, int newfd) override {
    printf("38 Captured dup2 call\n");
    api_count++;
    return 0;
  }

  int pipe(int pipefd[2]) override {
    printf("39 Captured pipe call\n");
    api_count++;
    return 0;
  }
  int mkfifo(const char *pathname, mode_t mode) override {
    printf("40 Captured mkfifo call\n");
    api_count++;
    return 0;
  }

  mode_t umask(mode_t mask) override {
    printf("41 Captured umask call\n");
    api_count++;
    return 0;
  }

  int access(const char *path, int amode) override {
    printf("42 Captured access call\n");
    api_count++;
    return 0;
  }

  int faccessat(int fd, const char *path, int amode, int flag) override {
    printf("43 Captured faccessat call\n");
    api_count++;
    return 0;
  }

  int remove(const char *pathname) override {
    printf("44 Captured remove call\n");
    api_count++;
    return 0;
  }

  int truncate(const char *pathname, off_t length) override {
    printf("45 Captured truncate call\n");
    api_count++;
    return 0;
  }

  int ftruncate(int fd, off_t length) override {
    printf("46 Captured ftruncate call\n");
    api_count++;
    return 0;
  }

  int execl(const char *pathname, const char *arg, ...) override {
    printf("47 Captured execl call\n");
    api_count++;
    return 0;
  }

  int execlp(const char *file, const char *arg, ...) override {
    printf("48 Captured execlp call\n");
    api_count++;
    return 0;
  }

  int execv(const char *pathname, char *const argv[]) override {
    printf("49 Captured execv call\n");
    api_count++;
    return 0;
  }

  int execvp(const char *file, char *const argv[]) override {
    printf("50 Captured execvp call\n");
    api_count++;
    return 0;
  }

  int execvpe(const char *file, char *const argv[],
              char *const envp[]) override {
    printf("51 Captured execvpe call\n");
    api_count++;
    return 0;
  }

  int fork() override {
    printf("52 Captured fork call\n");
    api_count++;
    return 0;
  }

  void _fini() override {
    printf("53 Captured _fini call\n");
    api_count++;
  }

  void exit(int status) override {
    BRAHMA_MAP_OR_FAIL(exit);
    printf("54 Captured exit with code %d\n", status);
    api_count++;
    __real_exit(0);
  }

  void *mmap(void *addr, size_t length, int prot, int flags, int fd,
             off_t offset) override {
    printf("55 Captured mmap call\n");
    api_count++;
    return 0;
  }

  void *mmap64(void *addr, size_t length, int prot, int flags, int fd,
               off64_t offset) override {
    printf("56 Captured mmap64 call\n");
    api_count++;
    return 0;
  }

  int munmap(void *addr, size_t len) override {
    printf("57 Captured munmap call\n");
    api_count++;
    return 0;
  }

  int msync(void *addr, size_t len, int flags) override {
    printf("58 Captured msync call\n");
    api_count++;
    return 0;
  }

  long sysconf(int name) override {
    printf("59 Captured sysconf call\n");
    api_count++;
    return 0;
  }

  int madvise(void *addr, size_t length, int advice) override {
    printf("60 Captured madvise call\n");
    api_count++;
    return 0;
  }

  int mprotect(void *addr, size_t length, int prot) override {
    printf("61 Captured mprotect call\n");
    api_count++;
    return 0;
  }

  int mlock(const void *addr, size_t len) override {
    printf("62 Captured mlock call\n");
    api_count++;
    return 0;
  }

  int munlock(const void *addr, size_t len) override {
    printf("63 Captured munlock call\n");
    api_count++;
    return 0;
  }

  int mlockall(int flags) override {
    printf("64 Captured mlockall call\n");
    api_count++;
    return 0;
  }

  int munlockall(void) override {
    printf("65 Captured munlockall call\n");
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
    printf("1 Captured fopen call\n");
    api_count++;
    return 0;
  }
  FILE *fopen64(const char *path, const char *mode) override {
    printf("2 Captured fopen64 call\n");
    api_count++;
    return 0;
  }
  int fclose(FILE *fp) override {
    printf("3 Captured fclose call\n");
    api_count++;
    return 0;
  }
  size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream) override {
    printf("4 Captured fread call\n");
    api_count++;
    return 0;
  }
  size_t fwrite(const void *ptr, size_t size, size_t nmemb,
                FILE *stream) override {
    printf("5 Captured fwrite call\n");
    api_count++;
    return 0;
  }
  long ftell(FILE *fp) override {
    printf("6 Captured ftell call\n");
    api_count++;
    return 0;
  }
  int fseek(FILE *stream, long offset, int whence) override {
    printf("7 Captured fseek call\n");
    api_count++;
    return 0;
  }
  FILE *fdopen(int fd, const char *mode) override {
    printf("8 Captured fdopen call\n");
    api_count++;
    return 0;
  }
  int fileno(FILE *stream) override {
    printf("9 Captured fileno call\n");
    api_count++;
    return 0;
  }
  FILE *tmpfile(void) override {
    printf("10 Captured tmpfile call\n");
    api_count++;
    return 0;
  }
  int fseeko(FILE *stream, off_t offset, int whence) override {
    printf("11 Captured fseeko call\n");
    api_count++;
    return 0;
  }
  off_t ftello(FILE *stream) override {
    printf("12 Captured ftello call\n");
    api_count++;
    return 0;
  }

  void clearerr(FILE *) override {
    printf("13 Captured clearerr call\n");
    api_count++;
  }

  int feof(FILE *) override {
    printf("14 Captured feof call\n");
    api_count++;
    return 0;
  }

  int ferror(FILE *) override {
    printf("15 Captured ferror call\n");
    api_count++;
    return 0;
  }

  int fflush(FILE *) override {
    printf("16 Captured fflush call\n");
    api_count++;
    return 0;
  }

  int fgetc(FILE *) override {
    printf("17 Captured fgetc call\n");
    api_count++;
    return 0;
  }

  int fgetpos(FILE *, fpos_t *) override {
    printf("18 Captured fgetpos call\n");
    api_count++;
    return 0;
  }

  char *fgets(char *, int, FILE *) override {
    printf("19 Captured fgets call\n");
    api_count++;
    return nullptr;
  }

  void flockfile(FILE *) override {
    printf("20 Captured flockfile call\n");
    api_count++;
  }

  int fputc(int, FILE *) override {
    printf("21 Captured fputc call\n");
    api_count++;
    return 0;
  }

  int fputs(const char *, FILE *) override {
    printf("22 Captured fputs call\n");
    api_count++;
    return 0;
  }

  FILE *freopen(const char *, const char *, FILE *) override {
    printf("23 Captured freopen call\n");
    api_count++;
    return nullptr;
  }

  int fsetpos(FILE *, const fpos_t *) override {
    printf("24 Captured fsetpos call\n");
    api_count++;
    return 0;
  }

  int ftrylockfile(FILE *) override {
    printf("25 Captured ftrylockfile call\n");
    api_count++;
    return 0;
  }

  void funlockfile(FILE *) override {
    printf("26 Captured funlockfile call\n");
    api_count++;
  }

  int getc(FILE *) override {
    printf("27 Captured getc call\n");
    api_count++;
    return 0;
  }

  int getc_unlocked(FILE *) override {
    printf("28 Captured getc_unlocked call\n");
    api_count++;
    return 0;
  }

  int getw(FILE *) override {
    printf("29 Captured getw call\n");
    api_count++;
    return 0;
  }

  int pclose(FILE *) override {
    printf("30 Captured pclose call\n");
    api_count++;
    return 0;
  }

  int putw(int, FILE *) override {
    printf("31 Captured putw call\n");
    api_count++;
    return 0;
  }

  void rewind(FILE *) override {
    printf("32 Captured rewind call\n");
    api_count++;
  }

  int setvbuf(FILE *, char *, int, size_t) override {
    printf("33 Captured setvbuf call\n");
    api_count++;
    return 0;
  }

  int ungetc(int, FILE *) override {
    printf("34 Captured ungetc call\n");
    api_count++;
    return 0;
  }
};
#ifdef BRAHMA_ENABLE_MPI
class MPIIOTest : public MPIIO {
 private:
  static std::shared_ptr<MPIIOTest> instance;

 public:
  MPIIOTest() : api_count(0) {}

  ~MPIIOTest() {}
  static std::shared_ptr<MPIIOTest> get_instance() {
    if (instance == nullptr) {
      instance = std::make_shared<MPIIOTest>();
      MPIIOTest::set_instance(instance);
    }
    return instance;
  }
  size_t api_count;

  int MPI_File_close(MPI_File *fh) override {
    api_count++;
    return 0;
  }

  int MPI_File_set_size(MPI_File fh, MPI_Offset size) override {
    api_count++;
    return 0;
  }

  int MPI_File_iread_at(MPI_File fh, MPI_Offset offset, void *buf, int count,
                        MPI_Datatype datatype, MPI_Request *request) override {
    api_count++;
    return 0;
  }

  int MPI_File_iread(MPI_File fh, void *buf, int count, MPI_Datatype datatype,
                     MPI_Request *request) override {
    api_count++;
    return 0;
  }

  int MPI_File_iread_shared(MPI_File fh, void *buf, int count,
                            MPI_Datatype datatype,
                            MPI_Request *request) override {
    api_count++;
    return 0;
  }

  int MPI_File_iwrite_at(MPI_File fh, MPI_Offset offset, const void *buf,
                         int count, MPI_Datatype datatype,
                         MPI_Request *request) override {
    api_count++;
    return 0;
  }

  int MPI_File_iwrite(MPI_File fh, const void *buf, int count,
                      MPI_Datatype datatype, MPI_Request *request) override {
    api_count++;
    return 0;
  }

  int MPI_File_iwrite_shared(MPI_File fh, const void *buf, int count,
                             MPI_Datatype datatype,
                             MPI_Request *request) override {
    api_count++;
    return 0;
  }

  int MPI_File_open(MPI_Comm comm, const char *filename, int amode,
                    MPI_Info info, MPI_File *fh) override {
    api_count++;
    return 0;
  }

  int MPI_File_read_all_begin(MPI_File fh, void *buf, int count,
                              MPI_Datatype datatype) override {
    api_count++;
    return 0;
  }

  int MPI_File_read_all(MPI_File fh, void *buf, int count,
                        MPI_Datatype datatype, MPI_Status *status) override {
    api_count++;
    return 0;
  }

  int MPI_File_read_at_all(MPI_File fh, MPI_Offset offset, void *buf, int count,
                           MPI_Datatype datatype, MPI_Status *status) override {
    api_count++;
    return 0;
  }

  int MPI_File_read_at_all_begin(MPI_File fh, MPI_Offset offset, void *buf,
                                 int count, MPI_Datatype datatype) override {
    api_count++;
    return 0;
  }

  int MPI_File_read_at(MPI_File fh, MPI_Offset offset, void *buf, int count,
                       MPI_Datatype datatype, MPI_Status *status) override {
    api_count++;
    return 0;
  }

  int MPI_File_read(MPI_File fh, void *buf, int count, MPI_Datatype datatype,
                    MPI_Status *status) override {
    api_count++;
    return 0;
  }

  int MPI_File_read_ordered_begin(MPI_File fh, void *buf, int count,
                                  MPI_Datatype datatype) override {
    api_count++;
    return 0;
  }

  int MPI_File_read_ordered(MPI_File fh, void *buf, int count,
                            MPI_Datatype datatype,
                            MPI_Status *status) override {
    api_count++;
    return 0;
  }

  int MPI_File_read_shared(MPI_File fh, void *buf, int count,
                           MPI_Datatype datatype, MPI_Status *status) override {
    api_count++;
    return 0;
  }

  int MPI_File_sync(MPI_File fh) override {
    api_count++;
    return 0;
  }

  int MPI_File_write_all_begin(MPI_File fh, const void *buf, int count,
                               MPI_Datatype datatype) override {
    api_count++;
    return 0;
  }

  int MPI_File_write_all(MPI_File fh, const void *buf, int count,
                         MPI_Datatype datatype, MPI_Status *status) override {
    api_count++;
    return 0;
  }

  int MPI_File_write_at_all_begin(MPI_File fh, MPI_Offset offset,
                                  const void *buf, int count,
                                  MPI_Datatype datatype) override {
    api_count++;
    return 0;
  }

  int MPI_File_write_at_all(MPI_File fh, MPI_Offset offset, const void *buf,
                            int count, MPI_Datatype datatype,
                            MPI_Status *status) override {
    api_count++;
    return 0;
  }

  int MPI_File_write_at(MPI_File fh, MPI_Offset offset, const void *buf,
                        int count, MPI_Datatype datatype,
                        MPI_Status *status) override {
    api_count++;
    return 0;
  }

  int MPI_File_write(MPI_File fh, const void *buf, int count,
                     MPI_Datatype datatype, MPI_Status *status) override {
    api_count++;
    return 0;
  }

  int MPI_File_write_ordered_begin(MPI_File fh, const void *buf, int count,
                                   MPI_Datatype datatype) override {
    api_count++;
    return 0;
  }

  int MPI_File_write_ordered(MPI_File fh, const void *buf, int count,
                             MPI_Datatype datatype,
                             MPI_Status *status) override {
    api_count++;
    return 0;
  }

  int MPI_File_write_shared(MPI_File fh, const void *buf, int count,
                            MPI_Datatype datatype,
                            MPI_Status *status) override {
    api_count++;
    return 0;
  }
  int MPI_File_delete(const char *filename, MPI_Info info) override {
    api_count++;
    return 0;
  }
};
#endif

std::shared_ptr<POSIXTest> POSIXTest::instance = nullptr;
std::shared_ptr<STDIOTest> STDIOTest::instance = nullptr;
#ifdef BRAHMA_ENABLE_MPI
std::shared_ptr<MPIIOTest> MPIIOTest::instance = nullptr;
#endif
}  // namespace brahma

void __attribute__((constructor)) test_init() {
  auto posix = brahma::POSIXTest::get_instance();
  posix->bind<brahma::POSIXTest>("tool", 1);
  auto stdio = brahma::STDIOTest::get_instance();
  stdio->bind<brahma::STDIOTest>("tool", 1);
#ifdef BRAHMA_ENABLE_MPI
  auto mpiio = brahma::MPIIOTest::get_instance();
  mpiio->bind<brahma::MPIIOTest>("tool", 1);
#endif
}
void __attribute__((destructor)) test_finalize() {
  printf("finalizing test\n");
  auto posix = brahma::POSIXTest::get_instance();
  size_t unbindings = posix->unbind();
  printf("POSIX num_bindings: %zu, api_count: %zu\n", posix->num_bindings,
         posix->api_count);
  printf("POSIX unbindings: %zu\n", unbindings);
  assert(posix->num_bindings == posix->api_count);
  auto stdio = brahma::STDIOTest::get_instance();
  printf("STDIO num_bindings: %zu, api_count: %zu\n", stdio->num_bindings,
         stdio->api_count);
  size_t stdio_unbindings = stdio->unbind();
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
#ifdef BRAHMA_ENABLE_MPI
  auto mpiio = brahma::MPIIOTest::get_instance();
  mpiio->unbind();
  assert(mpiio->num_bindings == mpiio->api_count);
#endif
}
#include <execinfo.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

void print_stacktrace(int sig) {
  void *array[32];
  size_t size = backtrace(array, 32);
  fprintf(stderr, "Error: signal %d\n", sig);
  backtrace_symbols_fd(array, size, STDERR_FILENO);
  exit(sig);
}

int main(int argc, char *argv[]) {
  signal(SIGSEGV, print_stacktrace);
  signal(SIGABRT, print_stacktrace);
  open("", 0);

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

  stat("", NULL);

  getcwd(NULL, 0);

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

  opendir("");

  readdir(NULL);

  closedir(NULL);

  rewinddir(NULL);

  fcntl(0, 0);

  dup(0);

  dup2(0, 0);

  pipe(NULL);

  mkfifo("", 0);

  umask(0);

  access("", 0);

  faccessat(0, "", 0, 0);

  remove("");

  truncate("", 0);

  ftruncate(0, 0);

  execl("", "");

  execlp("", "");

  execv("", NULL);

  execvp("", NULL);

  execvpe("", NULL, NULL);

  fork();

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
  fseeko(NULL, 0, 0);
  ftello(NULL);

  clearerr(NULL);
  feof(NULL);
  ferror(NULL);
  fflush(NULL);
  fgetc(NULL);
  fgetpos(NULL, 0);
  fgets(NULL, 0, NULL);
  fputc(0, NULL);
  fputs(NULL, NULL);
  freopen(NULL, NULL, NULL);
  fsetpos(NULL, NULL);
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

#ifdef BRAHMA_ENABLE_MPI

  MPI_File_close(NULL);

  MPI_File_set_size(NULL, 0);

  MPI_File_iread_at(NULL, 0, NULL, 0, 0, NULL);

  MPI_File_iread(NULL, NULL, 0, 0, NULL);

  MPI_File_iread_shared(NULL, NULL, 0, 0, NULL);

  MPI_File_iwrite_at(NULL, 0, NULL, 0, 0, NULL);

  MPI_File_iwrite(NULL, NULL, 0, 0, NULL);

  MPI_File_iwrite_shared(NULL, NULL, 0, 0, NULL);

  MPI_File_open(NULL, NULL, 0, 0, NULL);

  MPI_File_read_all_begin(NULL, NULL, 0, 0);

  MPI_File_read_all(NULL, NULL, 0, 0, NULL);

  MPI_File_read_at_all(NULL, 0, NULL, 0, 0, NULL);

  MPI_File_read_at_all_begin(NULL, 0, NULL, 0, 0);

  MPI_File_read_at(NULL, 0, NULL, 0, 0, NULL);

  MPI_File_read(NULL, NULL, 0, 0, NULL);

  MPI_File_read_ordered_begin(NULL, NULL, 0, 0);

  MPI_File_read_ordered(NULL, NULL, 0, 0, NULL);

  MPI_File_read_shared(NULL, NULL, 0, 0, NULL);

  MPI_File_sync(NULL);

  MPI_File_write_all_begin(NULL, NULL, 0, 0);

  MPI_File_write_all(NULL, NULL, 0, 0, NULL);

  MPI_File_write_at_all_begin(NULL, 0, NULL, 0, 0);

  MPI_File_write_at_all(NULL, 0, NULL, 0, 0, NULL);

  MPI_File_write_at(NULL, 0, NULL, 0, 0, NULL);

  MPI_File_write(NULL, NULL, 0, 0, NULL);

  MPI_File_write_ordered_begin(NULL, NULL, 0, 0);

  MPI_File_write_ordered(NULL, NULL, 0, 0, NULL);

  MPI_File_write_shared(NULL, NULL, 0, 0, NULL);
  MPI_File_delete(NULL, 0);
#endif
  exit(100);
  return 0;
}