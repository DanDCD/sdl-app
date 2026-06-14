#include "shader_cache.h"
#include "SDL3/SDL_filesystem.h"
#include "SDL3/SDL_gpu.h"
#include "SDL3/SDL_iostream.h"
#include "SDL3/SDL_log.h"
#include "SDL3/SDL_stdinc.h"
#include "SDL3_shadercross/SDL_shadercross.h"
#include <functional>
#include <string>
#include <vector>

ShaderCache::ShaderCache() : d_dirPath("./shaders") {
  SDL_Log("Booting ShaderCache");
}

bool ShaderCache::loadShaderFromFile(const std::string &filePath,
                                     SDL_GPUDevice &gpuDevice) {
  SDL_Log("Reading shader data from file %s", filePath.c_str());

  if (!filePath.contains("hlsl")) {
    SDL_Log("File %s is not in hlsl format - cannot read!", filePath.c_str());
    return false;
  }

  SDL_IOStream *fileIO = SDL_IOFromFile(filePath.c_str(), "r");
  if (!fileIO) {
    SDL_Log("Could not open file %s", filePath.c_str());
    return false;
  }

  Sint64 fileSize = SDL_GetIOSize(fileIO);
  std::string fileData(fileSize, '\0'); // \0 filled string of size fileSize

  auto readBytes = SDL_ReadIO(fileIO, &fileData, fileSize);

  if (readBytes == 0) {
    SDL_Log("Did not read any data from %s", filePath.c_str());
    return false;
  }

  SDL_Log("Read %zu bytes of %lld from %s", readBytes, fileSize,
          filePath.c_str());
  SDL_CloseIO(fileIO);

  return true;
}

bool ShaderCache::loadShadersFromDir(SDL_GPUDevice &gpuDevice) {
  SDL_ShaderCross_Init();
  SDL_Log("Loading Shaders from %s", d_dirPath.c_str());

  std::vector<std::string> shaderPaths;
  SDL_EnumerateDirectory(
      d_dirPath.c_str(),
      [](void *shaderPaths_vptr, const char *dirname, const char *fname) {
        auto shaderPaths_ptr = static_cast<std::vector<std::string>*>(shaderPaths_vptr);
        
        SDL_Log("Found shader file %s%s", dirname, fname);
        shaderPaths_ptr->push_back(std::string(dirname) + std::string(fname));

        return SDL_ENUM_CONTINUE;
      },
      &shaderPaths);

  for(const auto& path : shaderPaths)
      loadShaderFromFile(path, gpuDevice);

  SDL_ShaderCross_Quit();
  return true;
}