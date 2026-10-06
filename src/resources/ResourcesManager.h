#pragma once
#include <map>
#include <memory>
#include <string>

namespace Renderer {
class ShadeProgram;
}

class ResourcesManager {
public:
  ResourcesManager(const std::string &executablePath);

  ResourcesManager(const ResourcesManager &) = delete;
  ResourcesManager &operator=(const ResourcesManager &) = delete;
  ResourcesManager(ResourcesManager &&) = delete;
  ResourcesManager &operator=(ResourcesManager &&) = delete;

  std::shared_ptr<Renderer::ShadeProgram>
  loadShaderProgram(const std::string &shaderName, const std::string &vertexPath,
             const std::string &fragmentPath);
  std::shared_ptr<Renderer::ShadeProgram>
  getShaderProgram(const std::string &shaderName) const;

private:
  typedef std::map<const std::string, std::shared_ptr<Renderer::ShadeProgram>>
      ShadeProgramMap;

  std::string getFileString(const std::string &relativeFilePath) const;

  ShadeProgramMap m_shaders;
  std::string m_path;
};
