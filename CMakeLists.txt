cmake_minimum_required(VERSION 3.8)

project(NDIRestServer)

# find NDI
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules)
find_package(NDI REQUIRED)

# add the lib
add_library(NDIRestServer SHARED ${PROJECT_SOURCE_DIR}/src/NDIImageSender.cpp)

# include/links settings
target_include_directories(NDIRestServer PRIVATE ${NDI_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/include)
target_link_directories(NDIRestServer PRIVATE ${NDI_LIBRARY_DIR})
target_link_libraries(NDIRestServer PRIVATE ${NDI_LIBS})

# set output dirs of the target
set(OUT_DIR "${PROJECT_SOURCE_DIR}/bin/")
set_target_properties(NDIRestServer
  PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY ${OUT_DIR}
  ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${OUT_DIR}
  ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${OUT_DIR}

  LIBRARY_OUTPUT_DIRECTORY ${OUT_DIR}
  LIBRARY_OUTPUT_DIRECTORY_DEBUG ${OUT_DIR}
  LIBRARY_OUTPUT_DIRECTORY_RELEASE ${OUT_DIR}

  RUNTIME_OUTPUT_DIRECTORY ${OUT_DIR}
  RUNTIME_OUTPUT_DIRECTORY_DEBUG ${OUT_DIR}
  RUNTIME_OUTPUT_DIRECTORY_RELEASE ${OUT_DIR}
)

#if windows, copy the DLL to output
if (WIN32)
    file(COPY ${NDI_BIN_DIR}/Processing.NDI.Lib.x64.dll DESTINATION ${OUT_DIR}/)
endif()


# create C++ tester example
# add the execultabale
add_executable(tester 
    ${PROJECT_SOURCE_DIR}/src/tester.cpp
)
target_include_directories(tester PRIVATE ${NDI_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/include)
target_link_directories(tester PRIVATE ${OUT_DIR})
target_link_libraries(tester PRIVATE NDIRestServer)
set_target_properties(tester
  PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY ${OUT_DIR}
  ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${OUT_DIR}
  ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${OUT_DIR}

  LIBRARY_OUTPUT_DIRECTORY ${OUT_DIR}
  LIBRARY_OUTPUT_DIRECTORY_DEBUG ${OUT_DIR}
  LIBRARY_OUTPUT_DIRECTORY_RELEASE ${OUT_DIR}

  RUNTIME_OUTPUT_DIRECTORY ${OUT_DIR}
  RUNTIME_OUTPUT_DIRECTORY_DEBUG ${OUT_DIR}
  RUNTIME_OUTPUT_DIRECTORY_RELEASE ${OUT_DIR}
)