#include <iostream>
#include <ostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "resources/ResourcesManager.h"
#include "render/ShadeProgram.h"
#include "render/Texture2D.h"
#include "render/Mesh.h"
#include "render/Camera.h"

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


const std::vector<Renderer::Vertex> cubeVertices = {
    // Передняя грань (+Z)
    {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}},
    {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}},
    {{ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f}},
    {{-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f}},

    // Задняя грань (-Z)
    {{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
    {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},
    {{-0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}},
    {{ 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}},

    // Левая грань (-X)
    {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
    {{-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}},
    {{-0.5f,  0.5f,  0.5f}, {1.0f, 1.0f}},
    {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}},

    // Правая грань (+X)
    {{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}},
    {{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},
    {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}},
    {{ 0.5f,  0.5f,  0.5f}, {0.0f, 1.0f}},

    // Верхняя грань (+Y)
    {{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f}},
    {{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}},
    {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}},
    {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}},

    // Нижняя грань (-Y)
    {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
    {{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},
    {{ 0.5f, -0.5f,  0.5f}, {1.0f, 1.0f}},
    {{-0.5f, -0.5f,  0.5f}, {0.0f, 1.0f}},
};

const std::vector<GLuint> cubeIndices = {
    0,  1,  2,   2,  3,  0,
    4,  5,  6,   6,  7,  4,
    8,  9, 10,  10, 11,  8,
   12, 13, 14,  14, 15, 12,
   16, 17, 18,  18, 19, 16,
   20, 21, 22,  22, 23, 20,
};


int windowWidth = 640;
int windowHeight = 480;

void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        std::cout << "Escape key pressed" << std::endl;
    }
}


void processInput(GLFWwindow* window, Renderer::Camera& camera, float deltaTime) {
    auto pressed = [window](int key) { return glfwGetKey(window, key) == GLFW_PRESS; };

    glm::vec3 direction(0.0f);
    if (pressed(GLFW_KEY_W)) direction.z += 1.0f;
    if (pressed(GLFW_KEY_S)) direction.z -= 1.0f;
    if (pressed(GLFW_KEY_D)) direction.x += 1.0f;
    if (pressed(GLFW_KEY_A)) direction.x -= 1.0f;
    if (pressed(GLFW_KEY_SPACE)) direction.y += 1.0f;
    if (pressed(GLFW_KEY_LEFT_CONTROL)) direction.y -= 1.0f;

    const float boost = pressed(GLFW_KEY_LEFT_SHIFT) ? 4.0f : 1.0f;
    camera.move(direction, deltaTime * boost);
}

int main(int argc, char** argv)
{
    /* Initialize the library */
    if (!glfwInit()) {

        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    std::cout << "GLM Version: " << GLM_VERSION_MAJOR << "." << GLM_VERSION_MINOR << "." << GLM_VERSION_PATCH << "." << GLM_VERSION_REVISION << std::endl;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* pWindow = glfwCreateWindow(windowWidth, windowHeight, "FEMWheel", nullptr, nullptr);
    if (!pWindow)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetKeyCallback(pWindow, glfwKeyCallback);
    glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glfwRawMouseMotionSupported()) {
        glfwSetInputMode(pWindow, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(pWindow);

    if (!gladLoadGL()) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;


    {
    Renderer::Mesh cubeMesh(cubeVertices, cubeIndices);

        ResourcesManager resourcesManager(argv[0]);

    auto texture = resourcesManager.loadTexture(
        "asphalt",
        "res/textures/asphalt-1-1.jpg"
        );
    auto shader = resourcesManager.loadShaderProgram(
    "basic",
    "res/shaders/basic.vert",
    "res/shaders/basic.frag"
    );
    if (!shader || !texture) {
        std::cerr << "Failed to load rendering resources" << std::endl;
        glfwSetWindowShouldClose(pWindow, GLFW_TRUE);
    } else {
        shader->use();
        shader->setInt("u_texture", 0);
    }
    glEnable(GL_DEPTH_TEST);


    Renderer::Camera camera(glm::vec3(0.0f, 1.5f, 3.0f), -90.0f, -25.0f);

    double lastX, lastY;
    glfwGetCursorPos(pWindow, &lastX, &lastY);
    double lastTime = glfwGetTime();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(pWindow)) {
        int w, h;
        glfwGetFramebufferSize(pWindow, &w, &h);
        if (w == 0 || h == 0) { // окно свёрнуто, aspect был бы делением на ноль
            glfwWaitEvents();
            continue;
        }
        glViewport(0, 0, w, h);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const double now = glfwGetTime();
        const float deltaTime = static_cast<float>(now - lastTime);
        lastTime = now;

        double x, y;
        glfwGetCursorPos(pWindow, &x, &y);
        camera.rotate(static_cast<float>(x - lastX), static_cast<float>(y - lastY));
        lastX = x;
        lastY = y;

        processInput(pWindow, camera, deltaTime);

        float t = static_cast<float>(now);
        glm::mat4 model = glm::rotate(glm::mat4(1.0f), t, glm::vec3(0.5f, 1.0f, 0.0f));
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = camera.getProjectionMatrix(static_cast<float>(w) / static_cast<float>(h));


        shader->use();
        shader->setMat4("u_model", model);
        shader->setMat4("u_view", view);
        shader->setMat4("u_projection", projection);

        
        texture->bind(0);
        cubeMesh.draw();

        glfwSwapBuffers(pWindow);
        glfwPollEvents();
    }

    }

    glfwTerminate();
    return 0;
}
