#pragma once
#include <string>
#include <string_view>
#include <unordered_map>

#include "SDL3/SDL_gpu.h"
#include "SDL3_shadercross/SDL_shadercross.h"

class ShaderCache
{
public:
ShaderCache();
const std::string d_dirPath;

const std::unordered_map<std::string, SDL_GPUShader*>& vertexShaderCache() const;
const std::unordered_map<std::string, SDL_GPUShader*>& fragShaderCache() const;

bool loadShadersFromDir(SDL_GPUDevice& gpuDevice);

private:
bool loadShaderFromFile(const std::string& filePath, SDL_GPUDevice& gpuDevice);

std::unordered_map<std::string, SDL_GPUShader*> d_vertexShaderCache;
std::unordered_map<std::string, SDL_GPUShader*> d_fragShaderCache;
};
