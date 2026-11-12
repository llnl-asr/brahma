// A compat interface must be compiled in only for its own MPI implementation,
// major and minor version. This derives the expected family from the
// implementation and version the build was configured with and checks that the
// enabled set is exactly that family, or empty when no compat interface exists
// for it (which is what an MPI that brahma has never seen should get).
#include <brahma/brahma.h>

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <string>
#include <vector>

static std::string join(const std::vector<const char*>& v) {
  std::string out;
  for (auto* s : v) out += std::string(out.empty() ? "" : ", ") + s;
  return out.empty() ? "(none)" : out;
}

static int check(const char* name, const std::vector<const char*>& all,
                 const std::vector<const char*>& enabled,
                 const std::string& slug) {
  bool exists = std::find_if(all.begin(), all.end(), [&](const char* f) {
                  return slug == f;
                }) != all.end();
  std::printf("%s: build is %s; compat interfaces: %s\n", name, slug.c_str(),
              join(all).c_str());
  std::printf("%s: enabled: %s\n", name, join(enabled).c_str());
  if (exists) {
    if (enabled.size() != 1 || slug != enabled[0]) {
      std::printf("FAIL %s: expected only %s\n", name, slug.c_str());
      return 1;
    }
  } else if (!enabled.empty()) {
    std::printf("FAIL %s: expected no compat interface for %s\n", name,
                slug.c_str());
    return 1;
  }
  return 0;
}

int main() {
#if defined(BRAHMA_ENABLE_MPI) && defined(BRAHMA_MPI_IMPL_NAME) && \
    defined(BRAHMA_MPI_VERSION)
  std::string impl = BRAHMA_MPI_IMPL_NAME;
  std::transform(impl.begin(), impl.end(), impl.begin(),
                 [](unsigned char c) { return std::tolower(c); });
  const std::string slug = impl + "_" +
                           std::to_string(BRAHMA_MPI_VERSION / 100000) + "_" +
                           std::to_string((BRAHMA_MPI_VERSION / 100) % 1000);
  int failed = check("mpi", brahma::mpi_compat_families(),
                     brahma::mpi_compat_enabled(), slug);
  failed += check("mpiio", brahma::mpiio_compat_families(),
                  brahma::mpiio_compat_enabled(), slug);
  return failed == 0 ? 0 : 1;
#else
  std::printf("MPI implementation or version unknown; nothing to check\n");
  return 0;
#endif
}
