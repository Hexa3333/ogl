#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <GL/gl.h>

#include <cmath>
#include <iostream>
#include <cstdlib>

#include "shader.hpp"

constexpr int initial_window_width = 800;
constexpr int initial_window_height = 600;

void framebuffer_size_callback(struct GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void key_callback(struct GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    // Wireframe mode
    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
        static bool wireframe_enabled = false;
        if (wireframe_enabled) {
            wireframe_enabled = false;
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        } else {
            wireframe_enabled = true;
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
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


    GLfloat vertices[] = {
        0.0f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, // top
        0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 0.0f, // bot right
       -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f  // bot left
    };

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    Shader shader("shaders/main.vert", "shaders/main.frag");

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.5f, 0, 0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        float time_value = glfwGetTime();
        float green_value = (std::sin(time_value) / 2.0f) + 0.5f;

        glUseProgram(shader.program);
        glUniform3f(glGetUniformLocation(shader.program, "uni_color"),
                0.0f, 0.0f, 0.0f);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}
