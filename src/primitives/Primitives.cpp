#include "Primitives.h"

namespace Renderer {
    MeshData makeBox(float sx, float sy, float sz) {
        const glm::vec3 half(sx * 0.5f, sy * 0.5f, sz * 0.5f);

        struct Face { glm::vec3 n, u, v; };
        const Face faces[] = {
            {{ 0, 0,  1}, { 1, 0,  0}, {0, 1,  0}}, // +Z
            {{ 0, 0, -1}, {-1, 0,  0}, {0, 1,  0}}, // -Z
            {{-1, 0,  0}, { 0, 0,  1}, {0, 1,  0}}, // -X
            {{ 1, 0,  0}, { 0, 0, -1}, {0, 1,  0}}, // +X
            {{ 0, 1,  0}, { 1, 0,  0}, {0, 0, -1}}, // +Y
            {{ 0, -1, 0}, { 1, 0,  0}, {0, 0,  1}}, // -Y
        };
        const glm::vec2 corners[] = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};

        MeshData data;
        for (const Face& f : faces) {
            const auto base = static_cast<GLuint>(data.vertices.size());
            for (const glm::vec2& uv : corners) {
                const glm::vec3 p = (f.n + f.u * (uv.x * 2.0f - 1.0f) + f.v * (uv.y * 2.0f - 1.0f)) * half;
                data.vertices.push_back({p, uv, f.n});
            }
            data.indices.insert(data.indices.end(), {base, base + 1, base + 2, base + 2, base + 3, base});
        }
        return data;
    }

    MeshData makePlane(float size, float uvRepeat) {
        const float h = size * 0.5f;
        const glm::vec3 up(0.0f, 1.0f, 0.0f);

        MeshData data;
        data.vertices = {
            {{-h,0.0f,h}, {0.0f, 0.0f} , up},
            {{h,0.0f,h}, {uvRepeat, 0.0f} , up},
            {{h,0.0f,-h}, {uvRepeat, uvRepeat} , up},
            {{-h,0.0f,-h}, {0.0f, uvRepeat} , up}
        };

        data.indices = {0,1,2,  2,3,0};
        return data;
    }
}
