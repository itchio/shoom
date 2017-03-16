
#if defined(_WIN32)

#include <shoom.h>

#include <io.h>  // CreateFileMappingA, OpenFileMappingA, etc.

namespace shoom {

Shm::Shm(std::string path, size_t size) : path_(path), size_(size){};

ShoomError Shm::CreateOrOpen(bool create) {
  if (create) {
    handle_ = CreateFileMappingA(INVALID_HANDLE_VALUE,  // use paging file
                                 NULL,                  // default security
                                 PAGE_READWRITE,        // read/write access
                                 0,  // maximum object size (high-order DWORD)
                                 size,  // maximum object size (low-order DWORD)
                                 path.c_str()  // name of mapping object
                                 );

    if (!handle_) {
      return kErrorCreationFailed;
    }
  } else {
    handle_ = OpenFileMappingA(FILE_MAP_READ,  // read access
                               FALSE,          // do not inherit the name
                               path.c_str()    // name of mapping object
                               );

    if (!handle_) {
      return kErrorOpeningFailed;
    }
  }

  DWORD access = create ? FILE_MAP_ALL_ACCESS : FILE_MAP_READ;

  data_ = (char *)MapViewOfFile(handle_, access, 0, 0, size);

  if (!data_) {
    return kErrorMappingFailed;
  }

  return kOK;
}

/**
 * Destructor
 */
Shm::~Shm() {
  if (data_) {
    UnmapViewOfFile(data_);
    data_ = nullptr;
  }

  CloseHandle(handle_);
}

}  // namespace shoom

#endif  // _WIN32
