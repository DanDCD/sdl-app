#pragma once
#include "SDL3/SDL_gpu.h"

#include <memory>

struct SDL_GPUDevice_Deleter
{
    void operator()(SDL_GPUDevice* w) const;
};


class Graphics
{
public:
Graphics();
~Graphics();

public:
std::unique_ptr<SDL_GPUDevice, SDL_GPUDevice_Deleter> d_gpuDevice_ptr;
};