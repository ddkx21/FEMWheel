#pragma once
#include "glad/glad.h"

namespace Renderer {
    class Texture2D {
    public:
        Texture2D(int width, int height, const unsigned char *pixels);
        ~Texture2D();

        Texture2D operator = (Texture2D &) = delete;
        Texture2D(Texture2D &) = delete;
        Texture2D operator = (Texture2D &&) = delete;
        Texture2D(Texture2D &&) = delete;

        void bind(GLuint textureUnit = 0) const;

    private:
        GLuint m_ID = 0;
    };
}