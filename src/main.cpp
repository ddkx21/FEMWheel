#include <iostream>
#include <ostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "render/ShadeProgram.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


GLfloat cube[] = {

    -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,   0.2f, 0.2f, 0.2f,
};

GLuint indices[] = {
    0, 1, 2,  2, 3, 0,
    4, 5, 6,  6, 7, 4,
    0, 4, 7,  7, 3, 0,
    1, 5, 6,  6, 2, 1,
    3, 2, 6,  6, 7, 3,
    0, 1, 5,  5, 4, 0,
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

int main(void)
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
    GLuint vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

    auto shader = Renderer::ShadeProgram::fromFiles(
          std::string(RES_DIR) + "/shaders/basic.vert",
        std::string(RES_DIR) + "/shaders/basic.frag");

    if (!shader.isCompiled()) {
        std::cerr << "Cant create shader program!" << std::endl;
        return -1;
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

        shader.use();
        shader.setMat4("u_model", model);
        shader.setMat4("u_view", view);
        shader.setMat4("u_projection", projection);

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(pWindow);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    }

    glfwTerminate();
    return 0;
}