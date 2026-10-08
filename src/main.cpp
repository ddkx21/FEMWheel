#include <iostream>
#include <ostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "resources/ResourcesManager.h"
#include "render/ShadeProgram.h"
#include "render/Texture2D.h"
#include "render/Mesh.h"

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

void glfwWindowSizeCallback(GLFWwindow* window, int width, int height) {
    windowWidth = width;
    windowHeight = height;
    glViewport(0,0,windowWidth,windowHeight);
}

void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        std::cout << "Escape key pressed" << std::endl;
    }
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

    glfwSetWindowSizeCallback(pWindow, glfwWindowSizeCallback);
    glfwSetKeyCallback(pWindow, glfwKeyCallback);

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
    "res/basic.vert",
    "res/basic.frag"
    );
    if (!shader || !texture) {
        std::cerr << "Failed to load rendering resources" << std::endl;
        glfwSetWindowShouldClose(pWindow, GLFW_TRUE);
    } else {
        shader->use();
        shader->setInt("u_texture", 0);
    }
    glEnable(GL_DEPTH_TEST);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(pWindow)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        int w, h;
        glfwGetFramebufferSize(pWindow, &w, &h);
        glViewport(0, 0, w, h);

        float t = static_cast<float>(glfwGetTime());
        glm::mat4 model = glm::rotate(glm::mat4(1.0f), t, glm::vec3(0.5f, 1.0f, 0.0f));
        glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 1.5f, 3.0f),   // где камера
                                     glm::vec3(0.0f, 0.0f, 0.0f),   // куда смотрит
                                     glm::vec3(0.0f, 1.0f, 0.0f));  // где верх
        glm::mat4 projection = glm::perspective(glm::radians(60.0f),
                                                static_cast<float>(w) / static_cast<float>(h),
                                                0.1f, 100.0f);


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
