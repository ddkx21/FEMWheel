#pragma once

#include <string>
#include "glad/glad.h"
#include <glm/glm.hpp>


namespace Renderer {
    class ShadeProgram {
        public:
            ShadeProgram(const std::string& vertexShader, const std::string& fragmentShader);
            ~ShadeProgram();
            bool isCompiled() const{ return m_isCompiled; };
            void use() const;

            ShadeProgram() = delete;
            ShadeProgram(const ShadeProgram&) = delete;
            ShadeProgram& operator=(const ShadeProgram&) = delete;


            ShadeProgram& operator=(ShadeProgram&& ShadeProgram) noexcept;
            ShadeProgram(ShadeProgram&& ShadeProgram) noexcept;

            static ShadeProgram fromFiles(const std::string& vertexPath, const std::string& fragmentPath);
            void setMat4(const std::string& name, const glm::mat4& value) const;
            void setVec3(const std::string& name, const glm::vec3& value) const;

        private:
            static bool compileShader(const std::string& source,const GLuint shaderType, GLuint& shaderID);
            bool m_isCompiled = false;
            GLuint m_ID = 0;
    };
}

