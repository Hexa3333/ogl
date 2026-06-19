#version 330 core
out vec4 frag_color;
in vec2 tex_coord;

uniform sampler2D texture1;

uniform vec3 object_color;
uniform vec3 light_color;

void main()
{
    float ambient_strength = 0.1;
    vec3 ambient = ambient_strength * light_color;

    frag_color = texture(texture1, tex_coord) * vec4(ambient * object_color, 1.0);
} 
