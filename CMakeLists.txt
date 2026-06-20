cmake_minimum_required(VERSION 3.16)
project(universal-file-search VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_compile_options(-Wall -Wextra)

# --- Core library (everything except main.cpp) ---
add_library(ufs_core
    src/core/ParserRegistry.cpp
    src/parsers/TextParser.cpp
    src/parsers/LogParser.cpp
    src/engine/SearchEngine.cpp
)

target_include_directories(ufs_core PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

# --- Executable ---
add_executable(fsearch
    src/main.cpp
)

target_link_libraries(fsearch PRIVATE ufs_core)

# --- Tests (added in a later phase) ---
option(BUILD_TESTS "Build unit tests" OFF)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()