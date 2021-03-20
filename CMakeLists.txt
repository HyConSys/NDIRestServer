cmake_minimum_required(VERSION 3.8)

project(NDIRestServer)

# find NDI
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules)
find_package(NDI REQUIRED)


# add the execultabale
add_executable(NDIRestServer ${PROJECT_SOURCE_DIR}/src/main.cpp)

target_include_directories(NDIRestServer PRIVATE ${NDI_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/src)
target_link_directories(NDIRestServer PRIVATE ${NDI_LIBRARY_DIR})
target_link_libraries(NDIRestServer PRIVATE ${NDI_LIBS})