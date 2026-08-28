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

if(WIN32)
    # #pragma comment(lib, "Ws2_32.lib") in TcpServer.h only works under MSVC.
    # GCC/MinGW (e.g. MSYS2 UCRT64) silently ignores it, so Winsock must be
    # linked explicitly here or the link step fails with undefined references
    # to WSAStartup, socket, bind, etc.
    target_link_libraries(
        DistributedCache
        PRIVATE
        ws2_32
    )
endif()

set_target_properties(
    DistributedCache
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# ---- Tests (off by default; enable with -DBUILD_TESTS=ON) ----
option(BUILD_TESTS "Build test executables" OFF)

if(BUILD_TESTS)
    file(GLOB_RECURSE CACHE_ENGINE_SOURCES src/cache/*.cpp)
    file(GLOB_RECURSE STORAGE_SOURCES src/cache/storage/*.cpp)
    file(GLOB_RECURSE PROTOCOL_SOURCES src/protocol/*.cpp)
    file(GLOB_RECURSE CONCURRENCY_SOURCES src/concurrency/*.cpp)

    add_executable(
        cacheEngineTest
        tests/cacheEngineTest.cpp
        ${CACHE_ENGINE_SOURCES}
        ${STORAGE_SOURCES}
    )
    target_include_directories(cacheEngineTest PRIVATE include)

    add_executable(
        stressTest
        tests/stressTest.cpp
        ${CACHE_ENGINE_SOURCES}
        ${STORAGE_SOURCES}
        ${PROTOCOL_SOURCES}
        ${CONCURRENCY_SOURCES}
    )
    target_include_directories(stressTest PRIVATE include)
    target_link_libraries(stressTest PRIVATE Threads::Threads)

    # Build with -DENABLE_TSAN=ON (GCC/Clang only) to run stressTest under
    # ThreadSanitizer, e.g.: cmake .. -DBUILD_TESTS=ON -DENABLE_TSAN=ON
    option(ENABLE_TSAN "Build tests with ThreadSanitizer" OFF)
    if(ENABLE_TSAN AND NOT MSVC)
        target_compile_options(stressTest PRIVATE -fsanitize=thread -g)
        target_link_options(stressTest PRIVATE -fsanitize=thread)
    endif()

    set_target_properties(
        cacheEngineTest stressTest
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    )
endif()