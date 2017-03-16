
#if defined(_WIN32)

#include <shoom.h>

#include <io.h>  // CreateFileMappingA, OpenFileMappingA, etc.

namespace shoom {

/**
 * Constructor
 */
ShoomError Shm::CreateOrOpen(bool create) {
  if (create) {
    handle = CreateFileMappingA(INVALID_HANDLE_VALUE,  // use paging file
                                NULL,                  // default security
                                PAGE_READWRITE,        // read/write access
                                0,     // maximum object size (high-order DWORD)
                                size,  // maximum object size (low-order DWORD)
                                path.c_str()  // name of mapping object
                                );

    if (!handle) {
      return kErrorCreationFailed;
    }
  } else {
    handle = OpenFileMappingA(FILE_MAP_READ,  // read access
                              FALSE,          // do not inherit the name
                              path.c_str()    // name of mapping object
                              );

    if (!handle) {
      return kErrorOpeningFailed;
    }
  }

  DWORD dwAccess = create ? FILE_MAP_ALL_ACCESS : FILE_MAP_READ;

  mapped = (char *)MapViewOfFile(handle, dwAccess, 0, 0, size);

  if (!mapped) {
    return kMappingFailed;
  }

  return kNoError;
}

/**
 * Destructor
 */
Shm::~Shm() {
  if (mapped) {
    UnmapViewOfFile(mapped);
  }
  CloseHandle(handle);
}

}  // namespace shoom

#endif  // _WIN32
