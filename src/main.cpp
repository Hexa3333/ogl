#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <GL/gl.h>
#include <cstdlib>
#include <iostream>

constexpr int initial_window_width = 800;
constexpr int initial_window_height = 600;

void framebuffer_size_callback(struct GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void key_callback(struct GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

int main(void)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(initial_window_width, initial_window_height, "OGL", nullptr, nullptr);
    if (window == NULL) {
        std::cerr << "Failed to initiate glfw window.\n";
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize glad.\n";
        return EXIT_FAILURE;
    }

    glViewport(0, 0, initial_window_width, initial_window_height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.5f, 0, 0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}
