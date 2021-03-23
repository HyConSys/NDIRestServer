cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 11)

project(twoImages)

set(MAIN_PROJECT_DIR "${PROJECT_SOURCE_DIR}/../../..")
set(MAIN_PROJECT_BIN_DIR "${MAIN_PROJECT_DIR}/bin")

# find NDI
set(CMAKE_MODULE_PATH ${MAIN_PROJECT_DIR}/cmake/Modules)
find_package(NDI REQUIRED)

# create C++ twoImages example
# add the execultabale
add_executable(twoImages 
    ${PROJECT_SOURCE_DIR}/twoImages.cpp
)
target_include_directories(twoImages PRIVATE ${PROJECT_SOURCE_DIR} ${NDI_INCLUDE_DIR} ${MAIN_PROJECT_DIR}/include)
target_link_directories(twoImages PRIVATE ${MAIN_PROJECT_BIN_DIR})
target_link_libraries(twoImages PRIVATE NDIRestServer)


# copy images folder to output
file(COPY ${PROJECT_SOURCE_DIR}/images DESTINATION ${CMAKE_BINARY_DIR})