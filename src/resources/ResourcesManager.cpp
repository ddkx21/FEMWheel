#include "ResourcesManager.h"
#include "../render/ShadeProgram.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>


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
