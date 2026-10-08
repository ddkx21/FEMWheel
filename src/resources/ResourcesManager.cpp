#include "ResourcesManager.h"
#include "../render/ShadeProgram.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include "../render/Texture2D.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

ResourcesManager::ResourcesManager(const std::string &executablePath)
    : m_path(executablePath) {
  size_t found = m_path.find_last_of("/\\");
  m_path = m_path.substr(0, found);
}

std::string
ResourcesManager::getFileString(const std::string &relativeFilePath) const {
  std::fstream f;
  f.open(m_path + "/" + relativeFilePath.c_str(),
         std::ios::in | std::ios::binary);
  if (!f.is_open()) {
    std::cerr << "Failed to open file: " << m_path + "/" + relativeFilePath
              << std::endl;
    return std::string();
  }
  std::stringstream buffer;
  buffer << f.rdbuf();
  return buffer.str();
}

std::shared_ptr<Renderer::ShadeProgram>
ResourcesManager::loadShaderProgram(const std::string &shaderName,
                             const std::string &vertexPath,
                             const std::string &fragmentPath) {
  std::string vertexSource = getFileString(vertexPath);
  if (vertexSource.empty()) {
    std::cerr << "Failed to load vertex shader: " << vertexPath << std::endl;
    return nullptr;
  }

  std::string fragmentSource = getFileString(fragmentPath);
  if (fragmentSource.empty()) {
    std::cerr << "Failed to load fragment shader: " << fragmentPath
              << std::endl;
    return nullptr;
  }

  std::shared_ptr<Renderer::ShadeProgram>& newShader =
      m_shaders
          .emplace(shaderName, std::make_shared<Renderer::ShadeProgram>(
                                   vertexSource, fragmentSource))
          .first->second;
  if (!newShader->isCompiled()) {
    std::cerr << "Failed to compile shader program: " << shaderName
              << std::endl;
    m_shaders.erase(shaderName);
    return nullptr;
  }
  return newShader;
}

std::shared_ptr<Renderer::ShadeProgram>
  ResourcesManager::getShaderProgram(const std::string &shaderName) const{
     ShadeProgramMap::const_iterator it = m_shaders.find(shaderName);
      if (it != m_shaders.end()) {
          return it->second;
      }
      std::cerr << "Shader not found: " << shaderName << std::endl;
      return nullptr; 
  }


std::shared_ptr<Renderer::Texture2D>
  ResourcesManager::loadTexture(const std::string &textureName, const std::string &filePath) {
  auto it = m_textures.find(textureName);
  if (it != m_textures.end()) {
    return it->second;
  }

  const std::string fullPath = m_path + "/" + filePath;
  int width = 0, height = 0, channels = 0;
  stbi_set_flip_vertically_on_load(true);

  using ImageData = std::unique_ptr<unsigned char, decltype(&stbi_image_free)>;

  ImageData pixels(stbi_load(fullPath.c_str(), &width, &height, &channels, STBI_rgb_alpha),&stbi_image_free);

  if (!pixels) {
    std::cerr << "Failed to load texture: " << fullPath << std::endl;
    return nullptr;
  }

  auto texture = std::make_shared<Renderer::Texture2D>(width, height, pixels.get());
  m_textures.emplace(textureName, texture);
  return texture;
}

std::shared_ptr<Renderer::Texture2D>
  ResourcesManager::getTexture(const std::string &textureName) const {
  auto it = m_textures.find(textureName);
  if (it != m_textures.end()) {
    return it->second;
  }

  std::cerr << "Texture not found: " << textureName << std::endl;
  return nullptr;
}
