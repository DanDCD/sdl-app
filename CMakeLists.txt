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
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.15.2
    GIT_SHALLOW    TRUE
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(SDL3 googletest)

# ── Library ───────────────────────────────────────────────────────────────────

add_library(counter STATIC
    src/counter/Counter.cpp
)

add_library(app STATIC
    src/app/app_state.cpp
)

target_include_directories(counter PUBLIC src)
target_include_directories(app PUBLIC src)

target_link_libraries(app PUBLIC SDL3::SDL3-static)

# ── Application ───────────────────────────────────────────────────────────────

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE counter SDL3::SDL3-static app)

# ── Tests ─────────────────────────────────────────────────────────────────────

add_executable(tests tests/test_counter.cpp)
target_link_libraries(tests PRIVATE counter GTest::gmock_main)

include(GoogleTest)
gtest_discover_tests(tests)
