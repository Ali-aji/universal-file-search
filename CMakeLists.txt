cmake_minimum_required(VERSION 3.16)
project(universal-file-search VERSION 0.1.0 LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.4
)
FetchContent_MakeAvailable(Catch2)

FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(json)


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
    src/parsers/CsvParser.cpp
    src/parsers/JsonParser.cpp
    src/engine/SearchEngine.cpp
)

target_include_directories(ufs_core PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(ufs_core PUBLIC nlohmann_json::nlohmann_json)

# --- Executable ---
add_executable(ufs
    src/main.cpp
)

target_link_libraries(ufs PRIVATE ufs_core)

# --- Tests (added in a later phase) ---
option(BUILD_TESTS "Build unit tests" OFF)
if(BUILD_TESTS)
    enable_testing()
    include(${catch2_SOURCE_DIR}/extras/Catch.cmake)
    add_subdirectory(tests)
endif()