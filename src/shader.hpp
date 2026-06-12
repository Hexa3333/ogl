#pragma once

#include <string>
#include "GL/gl.h"

struct Shader {
    Shader(const std::string& vertex_shader_source_path,
           const std::string& fragment_shader_source_path);
    
    GLuint program;
};
