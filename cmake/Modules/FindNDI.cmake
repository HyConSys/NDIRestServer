if(WIN32)
  if(DEFINED ENV{NDI_SDK_DIR})
    set(NDI_FOUND TRUE)
    set(NDI_DIR $ENV{NDI_SDK_DIR})
    string(REPLACE "\\" "/" NDI_DIR "${NDI_DIR}")
    set(NDI_INCLUDE_DIR "${NDI_DIR}/Include")
    set(NDI_LIBRARY_DIR "${NDI_DIR}/Lib/x64")
    set(NDI_BIN_DIR "${NDI_DIR}/Bin/x64")
    set(NDI_LIBS "Processing.NDI.Lib.x64")
  else()
    set(NDI_FOUND FALSE)
  endif()
elseif(APPLE)
  if(EXISTS "/Library/NDI SDK for Apple/include/Processing.NDI.Lib.h")
    set(NDI_FOUND TRUE)
    set(NDI_DIR "/Library/NDI SDK for Apple")
    set(NDI_INCLUDE_DIR "${NDI_DIR}/include")
    set(NDI_LIBRARY_DIR "${NDI_DIR}/lib/x64")
    file(GLOB NDI_LIBS "${NDI_LIBRARY_DIR}/*.dylib")
  else()
    set(NDI_FOUND FALSE)
  endif()
elseif(UNIX)
  if(EXISTS "/usr/include/Processing.NDI.Lib.h")
    set(NDI_FOUND TRUE)
    set(NDI_DIR "/usr")
    set(NDI_INCLUDE_DIR "${NDI_DIR}/include")
    set(NDI_LIBRARY_DIR "${NDI_DIR}/lib")
    set(NDI_LIBS "ndi")
  else()
    set(NDI_FOUND FALSE)
 endif()
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(NDI DEFAULT_MSG NDI_DIR ${NDI_FOUND})