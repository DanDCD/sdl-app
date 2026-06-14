cmake_minimum_required(VERSION 3.25)
project(cpp_app VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)
include(FetchContent)

# ── Dependencies ──────────────────────────────────────────────────────────────

# SDL3
find_package(Vulkan REQUIRED) # assumes Vulkan is installed (see README)

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

# SDL Shadercross (note this makes binary huge, but useful for dev - pre-compiled shaders should be used for release)
FetchContent_Declare(
    sdl_shadercross
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_shadercross.git
    GIT_TAG        main
    GIT_SHALLOW    TRUE
)

# DXC ships as a submodule inside SDL_shadercross; these options pull it in
set(SDLSHADERCROSS_VENDORED        ON  CACHE BOOL "" FORCE)
set(SDLSHADERCROSS_ENABLE_INSTALL  OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(SDL3 sdl_shadercross googletest)

# ── Library ───────────────────────────────────────────────────────────────────

add_library(counter STATIC
    src/counter/Counter.cpp
)

add_library(app STATIC
    src/app/app_state.cpp
    src/app/graphics.cpp
    src/app/shader_cache.cpp
)

target_include_directories(counter PUBLIC src)
target_include_directories(app PUBLIC src)

target_link_libraries(app PUBLIC SDL3::SDL3-static SDL3_shadercross::SDL3_shadercross Vulkan::Vulkan)

# ── Application ───────────────────────────────────────────────────────────────

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE app)

# ── Tests ─────────────────────────────────────────────────────────────────────

add_executable(tests tests/test_counter.cpp)
target_link_libraries(tests PRIVATE counter GTest::gmock_main)

include(GoogleTest)
gtest_discover_tests(tests)

# ── Post ───────────────────────────────────────────────────────────────────────

# Shader Files
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/shaders"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders"
    COMMENT "Copying shaders to build output"
)