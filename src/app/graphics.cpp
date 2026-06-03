#include "graphics.h"
#include "SDL3/SDL_error.h"
#include "SDL3/SDL_gpu.h"
#include "SDL3/SDL_log.h"
#include "SDL3/SDL_video.h"
#include <cstdlib>

void SDL_GPUDevice_Deleter::operator()(SDL_GPUDevice* gpuDevice) const
{
    SDL_DestroyGPUDevice(gpuDevice);
}

/// TODO: allow togglable debug_mode
Graphics::Graphics(SDL_Window* window_ptr)
:   d_gpuDevice_ptr{SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, true, nullptr)}
{
    if (!d_gpuDevice_ptr) {
        SDL_Log("Failed to create GPU device: %s", SDL_GetError());
        return;
    }
    SDL_Log("GPU backend: %s", SDL_GetGPUDeviceDriver(d_gpuDevice_ptr.get()));

    auto windowClaimed = SDL_ClaimWindowForGPUDevice(d_gpuDevice_ptr.get(), window_ptr);
    if(!windowClaimed)
    {
        SDL_Log("Failed to claim window for GPU device: %s", SDL_GetError());
        return;
    }

    SDL_Log("Graphics component has finished setup");
}

Graphics::~Graphics()
{
    
}