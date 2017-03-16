
#if defined(__linux__) || defined(__APPLE__)

#include <shoom.h>

#include <fcntl.h>     // for O_* constants
#include <sys/mman.h>  // mmap, munmap
#include <sys/stat.h>  // for mode constants
#include <unistd.h>    // unlink

#include <stdexcept>

namespace shoom {

ShoomError Shm::CreateOrOpen(bool create) {
  if (create) {
    // shm segments persist across runs, and macOS will refuse
    // to ftruncate an existing shm segment, so to be on the safe
    // side, we unlink it beforehand.
    // TODO(amos) check errno while ignoring ENOENT?
    int ret = shm_unlink(path_.c_str());
    if (ret < 0) {
      if (errno != ENOENT) {
        return kErrorCreationFailed;
      }
    }
  }

  int flags = create ? (O_CREAT | O_RDWR) : O_RDONLY;

  fd_ = shm_open(path_.c_str(), flags, 0755);
  if (fd_ < 0) {
    if (create) {
      return kErrorCreationFailed;
    } else {
      return kErrorOpeningFailed;
    }
  }

  if (create) {
    // this is the only way to specify the size of a
    // newly-created POSIX shared memory object
    int ret = ftruncate(fd_, size_);
    if (ret != 0) {
      return kErrorCreationFailed;
    }
  }

  int prot = create ? (PROT_READ | PROT_WRITE) : PROT_READ;

  mapped_ = (char *)mmap(nullptr,     // addr
                         size_,       // length
                         prot,        // prot
                         MAP_SHARED,  // flags
                         fd_,         // fd
                         0            // offset
                         );

  if (!mapped_) {
    throw std::runtime_error("mmap failed");
  }
}

Shm::~Shm() {
  munmap(mapped_, size_);
  close(fd_);
}

}  // namespace shoom

#endif  // __linux__ or __APPLE__
