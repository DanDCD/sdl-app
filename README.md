# SDL3 App

A minimal C++23 SDL3 application — an 800x600 window with a dark background.

## Prerequisites

```sh
brew install cmake ninja pkg-config
brew install vulkan-headers vulkan-loader vulkan-validationlayers vulkan-tools molten-vk # for vulkan SDK (Debug Mode)
```

## Build

```sh
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

SDL3 is fetched from source automatically on first configure (~1 min).

## Run

```sh
export VK_LAYER_PATH=$(brew --prefix vulkan-validationlayers)/share/vulkan/explicit_layer.d # if debug mode on for Vulkan
./build/cpp_app
```

## Resources
https://scratchapixel.com/
https://gpuforbeginners.com/
https://wiki.libsdl.org/SDL3/Tutorials
https://moonside.games/posts/layers-all-the-way-down/
https://moonside.games/posts/introducing-sdl-shadercross/