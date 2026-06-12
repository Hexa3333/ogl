#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <GL/gl.h>

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>

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
        // first tri
        0.5f,  0.5f,  0.0f, // top right
        0.5f, -0.5f,  0.0f, // bot right
       -0.5f,  0.5f,  0.0f, // top left
       -0.5f, -0.5f,  0.0f, // bot left
    };

    GLuint indices[] = {
        0, 1, 2, // first tri
        2, 3, 1  // second tri
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    // Read vshader src
    std::ifstream vertex_shader_file_stream("shaders/main.vert");
    std::stringstream vertex_shader_stringstream;
    vertex_shader_stringstream << vertex_shader_file_stream.rdbuf();
    vertex_shader_file_stream.close();
    std::string vertex_shader_code_str = vertex_shader_stringstream.str();
    const GLchar* vertex_shader_code = vertex_shader_code_str.c_str();

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_code, nullptr);
    glCompileShader(vertex_shader);
    {
        int success;
        char info_log[512];
        glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertex_shader, 512, nullptr, info_log);
            std::cerr << "Vertex::Compilation shader error: " << info_log << "\n";
        }
    }

    // Read fshader src
    std::ifstream fragment_shader_file_stream("shaders/main.frag");
    std::stringstream fragment_shader_stringstream;
    fragment_shader_stringstream << fragment_shader_file_stream.rdbuf();
    fragment_shader_file_stream.close();
    std::string fragment_shader_code_str = fragment_shader_stringstream.str();
    const GLchar* fragment_shader_code = fragment_shader_code_str.c_str();

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_code, nullptr);
    glCompileShader(fragment_shader);
    {
        int success;
        char info_log[512];
        glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragment_shader, 512, nullptr, info_log);
            std::cerr << "Fragment::Compilation shader error: " << info_log << "\n";
        }
    }

    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    {
        int success;
        char info_log[512];
        glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader_program, 512, nullptr, info_log);
            std::cerr << "Shader linking error: " << info_log << "\n";
        }
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    glUseProgram(shader_program);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.5f, 0, 0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_program);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}
