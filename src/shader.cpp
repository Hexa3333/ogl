#include "glad/glad.h"
#include "shader.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string& vertex_shader_source_path,
               const std::string& fragment_shader_source_path) {
    // Read vshader src
    std::ifstream vertex_shader_file_stream(vertex_shader_source_path);
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
            std::cerr << "Vertex::Compilation shader error: " << info_log << "n";
        }
    }

    // Read fshader src
    std::ifstream fragment_shader_file_stream(fragment_shader_source_path);
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


    // Program
    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    {
        int success;
        char info_log[512];
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(program, 512, nullptr, info_log);
            std::cerr << "Shader linking error: " << info_log << "\n";
        }
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}
