#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <GL/gl.h>

#include <cmath>
#include <iostream>
#include <cstdlib>

#include "shader.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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
//       v1     v2    v3      r     g     b      s     t
        0.5f,  0.5f, 0.0f,   0.0f, 0.5f, 0.0f,  1.0f, 1.0f, // top right
       -0.5f,  0.5f, 0.0f,   0.0f, 0.5f, 0.0f,  0.0f, 1.0f, // top left
        0.5f, -0.5f, 0.0f,   0.0f, 0.5f, 0.0f,  1.0f, 0.0f, // bot right
       -0.5f, -0.5f, 0.0f,   0.0f, 0.5f, 0.0f,  0.0f, 0.0f, // bot left
    };
    GLuint indices[] = {
        0, 1, 2,
        2, 1, 3
    };

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    Shader shader("shaders/main.vert", "shaders/main.frag");

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char *data = stbi_load("textures/wall.jpg", &width, &height, &nrChannels, 0);
    if (!data) {
        std::cerr << "stbi failed.\n";
        return EXIT_FAILURE;
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.5f, 0, 0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        float time_value = glfwGetTime();
        float green_value = (std::sin(time_value) / 2.0f) + 0.5f;

        glUseProgram(shader.program);
        glUniform3f(glGetUniformLocation(shader.program, "uni_color"),
                0.0f, 0.0f, 0.0f);
        glBindVertexArray(vao);
        glBindTexture(GL_TEXTURE_2D, texture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}
