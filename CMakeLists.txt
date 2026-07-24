cmake_minimum_required(VERSION 3.16)

project(
    DistributedCache
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -pthread)
endif()

include_directories(include)

file(GLOB_RECURSE SOURCE_FILES
    src/*.cpp
)

add_executable(
    DistributedCache
    ${SOURCE_FILES}
)

target_include_directories(
    DistributedCache
    PRIVATE
    include
)

find_package(Threads REQUIRED)

target_link_libraries(
    DistributedCache
    PRIVATE
    Threads::Threads
)

set_target_properties(
    DistributedCache
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)