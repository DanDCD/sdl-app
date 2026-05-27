#include "graphics.h"
#include "SDL3/SDL_log.h"
#include <cstdlib>

void SDL_GPUDevice_Deleter::operator()(SDL_GPUDevice* gpuDevice) const
{

}


Graphics::Graphics()
:   d_gpuDevice_ptr{SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, true, nullptr)}
{
    if (!d_gpuDevice_ptr) {
        SDL_Log("Failed to create GPU device: %s", SDL_GetError());
        return;
    }
    SDL_Log("GPU backend: %s", SDL_GetGPUDeviceDriver(d_gpuDevice_ptr.get()));
}

Graphics::~Graphics()
{
    
}