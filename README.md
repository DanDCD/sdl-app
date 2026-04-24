# SDL3 App

A minimal C++23 SDL3 application — an 800x600 window with a dark background.

## Prerequisites

```sh
brew install cmake ninja pkg-config
```

## Build

```sh
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

SDL3 is fetched from source automatically on first configure (~1 min).

## Run

```sh
./build/cpp_app
```
