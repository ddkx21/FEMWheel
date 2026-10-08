#pragma once
#include <vector>

#include "glad/glad.h"
#include "glm/glm.hpp"

namespace Renderer {

    struct Vertex {
        glm::vec3 position;
        glm::vec2 uv;
    };

    class Mesh {
    public:
        Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices);

        ~Mesh();

        Mesh(const Mesh&) = delete;
        Mesh& operator=(const Mesh&) = delete;
        void draw() const;
    private:
        GLuint m_vao = 0, m_vbo = 0, m_ebo = 0;
        GLsizei m_indexCount = 0;
    };
}


