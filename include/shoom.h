#pragma once

#include <cstdint>
#include <string>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN
#endif  // _WIN32

namespace shoom {

enum ShoomError {
  kOK = 0,
  kErrorCreationFailed = 100,
  kErrorMappingFailed = 110,
  kErrorOpeningFailed = 120,
};

class Shm {
 public:
  explicit Shm(std::string path, size_t size) :
    path_(path), size_(size) {};
  ShoomError Create();
  ShoomError Open();
  ~Shm();

 private:
  ShoomError CreateOrOpen(bool create);

  std::string path_;
  char *mapped_;
  size_t size_;
#if defined(CAPSULE_WINDOWS)
  HANDLE handle_;
#else
  int fd_;
#endif
};

}