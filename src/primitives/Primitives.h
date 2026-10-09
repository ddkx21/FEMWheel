#pragma once
#include <vector>

#include "../render/Mesh.h"



namespace Renderer {
    struct MeshData {
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;
    };

    MeshData makePlane(float size, float uvRepeat);
    MeshData makeBox(float sx, float sy, float sz);
    MeshData makeCylinder(float radius, float width, int segments);
    MeshData makeSphere(float radius, int segments);

}
