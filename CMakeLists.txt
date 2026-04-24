cmake_minimum_required(VERSION 3.25)
project(cpp_app VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)
include(FetchContent)

# ── Dependencies ──────────────────────────────────────────────────────────────

FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG        main
    GIT_SHALLOW    TRUE
)

set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON  CACHE BOOL "" FORCE)
set(SDL_TEST   OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.7.0
    GIT_SHALLOW    TRUE
)

FetchContent_MakeAvailable(SDL3 Catch2)

# ── Library ───────────────────────────────────────────────────────────────────

add_library(counter STATIC
    src/counter/Counter.cpp
)
target_include_directories(counter PUBLIC src)

# ── Application ───────────────────────────────────────────────────────────────

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE counter SDL3::SDL3-static)

# ── Tests ─────────────────────────────────────────────────────────────────────

add_executable(tests tests/test_counter.cpp)
target_link_libraries(tests PRIVATE counter Catch2::Catch2WithMain)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch)
catch_discover_tests(tests)
