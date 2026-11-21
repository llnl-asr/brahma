// unbind() runs while another thread is inside an intercepted call, as when a
// tool unbinds at exit and a runtime thread (such as the HSA thread in ioctl)
// is still running. The live wrapper must keep finding the real function.
#include <brahma/brahma.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

#include <atomic>

namespace brahma {
class RaceTest : public POSIX {
  static std::shared_ptr<RaceTest> instance;

 public:
  static std::shared_ptr<RaceTest> get_instance() {
    if (instance == nullptr) {
      instance = std::make_shared<RaceTest>();
      POSIX::set_instance(instance);
    }
    return instance;
  }
  int close(int fd) override {
    BRAHMA_MAP_OR_FAIL(close);
    return __real_close(fd);
  }
};
std::shared_ptr<RaceTest> RaceTest::instance = nullptr;
}  // namespace brahma

static std::atomic<bool> stop{false};

static void *call_close(void *) {
  while (!stop.load(std::memory_order_relaxed)) close(-1);
  return nullptr;
}

// One bind, a thread in close(), then unbind. Runs in a child so a crash is
// counted instead of ending the test. A broken unbind can also leave the
// thread looping in the wrapper, so the alarm turns a hang into a failure.
static void run_once() {
  alarm(10);
  auto posix = brahma::RaceTest::get_instance();
  posix->bind<brahma::RaceTest>("race", 1);
  pthread_t thread;
  pthread_create(&thread, nullptr, call_close, nullptr);
  usleep(1000);
  posix->unbind();
  usleep(1000);
  stop = true;
  pthread_join(thread, nullptr);
  _exit(0);
}

int main() {
  const int runs = 200;
  for (int i = 0; i < runs; i++) {
    pid_t pid = fork();
    if (pid == 0) run_once();
    int status = 0;
    waitpid(pid, &status, 0);
    if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
      printf("unbind race: run %d of %d failed\n", i + 1, runs);
      return 1;
    }
  }
  printf("unbind race: %d runs passed\n", runs);
  return 0;
}
