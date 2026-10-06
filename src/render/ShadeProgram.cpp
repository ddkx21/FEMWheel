#include "ShadeProgram.h"
#include <iostream>
#include <ostream>
#include <fstream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>

namespace {
    std::string readFile(const std::string& path) {
        std::ifstream file(path);
        if (!file) {
            std::cerr << "Cannot open shader file: " << path << std::endl;
            return {};
        }
        std::stringstream ss;
        ss << file.rdbuf();
        return ss.str();
    }
}

namespace Renderer {
    ShadeProgram::ShadeProgram(const std::string& vertexShader, const std::string& fragmentShader) {
        GLuint vertexShaderID;
        if (!compileShader(vertexShader, GL_VERTEX_SHADER , vertexShaderID)) {
            std::cerr << "Error compiling VERTEX shader" << std::endl;
            return;
        }

        GLuint fragmentShaderID;
        if (!compileShader(fragmentShader, GL_FRAGMENT_SHADER , fragmentShaderID)) {
            std::cerr << "Error compiling FRAGMENT shader" << std::endl;
            glDeleteShader(vertexShaderID);
            return;
        }


        m_ID = glCreateProgram();
        glAttachShader(m_ID, vertexShaderID);
        glAttachShader(m_ID, fragmentShaderID);
        glLinkProgram(m_ID);

        GLint success;
        glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
        if (success == GL_FALSE) {
            GLchar infoLogLength[1024];
            glGetProgramInfoLog(m_ID, 1024, nullptr, infoLogLength);
            std::cerr << "Error compiling shader LINK: " << infoLogLength << std::endl;
        }
        else {
            m_isCompiled = true;
        }

        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);
    }


    bool ShadeProgram::compileShader(const std::string& source,const GLuint shaderType, GLuint& shaderID) {
        shaderID = glCreateShader(shaderType);
        const char* src = source.c_str();
        glShaderSource(shaderID, 1, &src, nullptr);
        glCompileShader(shaderID);

        GLint success;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
        if (success == GL_FALSE) {
            GLchar infoLogLength[1024];
            glGetShaderInfoLog(shaderID, 1024, nullptr, infoLogLength);
            std::cerr << "Error compiling shader: " << infoLogLength << std::endl;

            glDeleteShader(shaderID);
            return false;
        }
        return true;
    }

    ShadeProgram::~ShadeProgram() {
        glDeleteProgram(m_ID);
    }

    void ShadeProgram::use() const  {
        glUseProgram(m_ID);
    }

    ShadeProgram& ShadeProgram::operator=(ShadeProgram&& ShadeProgram) noexcept {
        if (this == &ShadeProgram) {
            return *this;
        }
        glDeleteProgram(m_ID);
        m_ID = ShadeProgram.m_ID;
        m_isCompiled = ShadeProgram.m_isCompiled;

        ShadeProgram.m_ID = 0;
        ShadeProgram.m_isCompiled = false;
        return *this;
    }

    ShadeProgram::ShadeProgram(ShadeProgram&& ShadeProgram) noexcept {
        m_ID = ShadeProgram.m_ID;
        m_isCompiled = ShadeProgram.m_isCompiled;

        ShadeProgram.m_ID = 0;
        ShadeProgram.m_isCompiled = false;
    }

    void ShadeProgram::setMat4(const std::string& name, const glm::mat4& value) const {
        glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
    }

    void ShadeProgram::setVec3(const std::string& name, const glm::vec3& value) const {
        glUniform3fv(glGetUniformLocation(m_ID, name.c_str()), 1, glm::value_ptr(value));
    }
}
