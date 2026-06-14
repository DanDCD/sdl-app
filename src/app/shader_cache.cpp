#include "shader_cache.h"
#include "SDL3/SDL_filesystem.h"
#include "SDL3/SDL_gpu.h"
#include "SDL3/SDL_iostream.h"
#include "SDL3/SDL_log.h"
#include "SDL3/SDL_stdinc.h"
#include "SDL3_shadercross/SDL_shadercross.h"
#include <cstddef>
#include <optional>
#include <string>
#include <vector>

ShaderCache::ShaderCache() : d_dirPath("./shaders") {
  SDL_Log("Booting ShaderCache");
}

bool ShaderCache::loadShaderFromFile(const std::string &filePath,
                                     SDL_GPUDevice &gpuDevice) {
  // Load Shader sourcecode
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

  auto readBytes = SDL_ReadIO(fileIO, fileData.data(), fileSize);
  SDL_CloseIO(fileIO);

  if (readBytes == 0) {
    SDL_Log("Did not read any data from %s", filePath.c_str());
    return false;
  }

  SDL_Log("Read %zu bytes of %lld from %s", readBytes, fileSize,
          filePath.c_str());
  SDL_Log("Loaded HLSL file data:\n%s", fileData.c_str());

  // Compile Shader source code
  SDL_Log("Compiling Shader from loaded data");

  auto shaderStage = inferStageFromPath(filePath);
  if (!shaderStage) {
    SDL_Log("No known shader stage for file extension in %s", filePath.c_str());
    return false;
  }

  // Defines is how we can insert variables into a shader:
  auto shaderVar = std::string{"SomeShaderVar"};
  auto shaderVal = std::string{"SomeShaderVal"};
  SDL_ShaderCross_HLSL_Define shaderDefs[] = {
      {shaderVal.data(), shaderVal.data()}, {nullptr, nullptr}};

  SDL_ShaderCross_HLSL_Info hlslInfo = {
      .source = fileData.c_str(),
      .entrypoint = shaderStage == SDL_SHADERCROSS_SHADERSTAGE_VERTEX
                        ? "VSMain"
                        : "PSMain",
      .include_dir = nullptr,
      .defines = shaderDefs,
      .shader_stage = shaderStage.value(),
  };

  // HLSL into SPIRV
  size_t compiledSPIRVSize;
  auto sprivBytes = static_cast<Uint8 *>(
      SDL_ShaderCross_CompileSPIRVFromHLSL(&hlslInfo, &compiledSPIRVSize));

  if (!compiledSPIRVSize || !sprivBytes) {
    SDL_Log("Failed to compile HLSL to SPIRV - is HLSL source valid?: %s",
            SDL_GetError());
    return false;
  }
  SDL_Log("Compiled SPIRV size: %zu", compiledSPIRVSize);

  // metadata->inputs and metadata->outputs tell you what vertex attributes the
  // shader expects
  auto sprivMetadata_ptr =
      SDL_ShaderCross_ReflectGraphicsSPIRV(sprivBytes, compiledSPIRVSize, 0);

  if (!sprivMetadata_ptr) {
    SDL_Log("Failed to reflect SPRIV: %s", SDL_GetError());
    SDL_free(sprivBytes);
    return false;
  }

  SDL_ShaderCross_SPIRV_Info spirvInfo = {.bytecode = sprivBytes,
                                          .bytecode_size = compiledSPIRVSize,
                                          .entrypoint = hlslInfo.entrypoint,
                                          .shader_stage = shaderStage.value(),
                                          .props = 0};

  // SPIRV into GPU Shader
  SDL_GPUShader *shader_ptr = SDL_ShaderCross_CompileGraphicsShaderFromSPIRV(
      &gpuDevice, &spirvInfo, &sprivMetadata_ptr->resource_info, 0);

  if (!shader_ptr) {
    SDL_Log("Failed to compile Graphics Shader from SPIRV: %s", SDL_GetError());
    return false;
  }

  SDL_free(sprivBytes);
  SDL_free(sprivMetadata_ptr);

  // Add GPU Shader to map
  SDL_Log("Successfully created %s GPU Shader from %s. Saving to Map.",
          shaderStage == SDL_SHADERCROSS_SHADERSTAGE_VERTEX ? "Vertex"
                                                            : "Fragment",
          filePath.c_str());
  if (shaderStage == SDL_SHADERCROSS_SHADERSTAGE_VERTEX)
    d_vertexShaderCache[filePath] = shader_ptr;
  else
    d_fragShaderCache[filePath] = shader_ptr;

  return true;
};

std::optional<SDL_ShaderCross_ShaderStage>
ShaderCache::inferStageFromPath(const std::string &filePath) const {
  if (filePath.ends_with(".vert.hlsl"))
    return SDL_SHADERCROSS_SHADERSTAGE_VERTEX;
  if (filePath.ends_with(".frag.hlsl"))
    return SDL_SHADERCROSS_SHADERSTAGE_FRAGMENT;
  // could add .comp.hlsl etc. later
  return std::nullopt;
}

bool ShaderCache::loadShadersFromDir(SDL_GPUDevice &gpuDevice) {
  SDL_ShaderCross_Init();
  SDL_Log("Loading Shaders from %s", d_dirPath.c_str());

  std::vector<std::string> shaderPaths;
  SDL_EnumerateDirectory(
      d_dirPath.c_str(),
      [](void *shaderPaths_vptr, const char *dirname, const char *fname) {
        auto shaderPaths_ptr =
            static_cast<std::vector<std::string> *>(shaderPaths_vptr);

        SDL_Log("Found shader file %s%s", dirname, fname);
        shaderPaths_ptr->push_back(std::string(dirname) + std::string(fname));

        return SDL_ENUM_CONTINUE;
      },
      &shaderPaths);

  for (const auto &path : shaderPaths)
    loadShaderFromFile(path, gpuDevice);

  SDL_ShaderCross_Quit();
  return true;
}