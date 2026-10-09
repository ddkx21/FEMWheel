#include "Texture2D.h"

#include <iostream>
#include <ostream>

namespace Renderer {
    Texture2D::Texture2D(int width, int height, const unsigned char *pixels) {
        if (width <= 0 || height <= 0 || pixels == nullptr) {
            std::cerr << "Texture load err" << std::endl;
            return;
        }

        glGenTextures(1, &m_ID);
        glBindTexture(GL_TEXTURE_2D, m_ID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

        glGenerateMipmap(GL_TEXTURE_2D);
    }
    Texture2D::~Texture2D() {
        glDeleteTextures(1, &m_ID);
    }

    void Texture2D::bind(GLuint unit) const {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, m_ID);
    }

}
