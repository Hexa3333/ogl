#version 330 core
out vec4 frag_color;
in vec2 tex_coord;
in vec3 normal;
in vec3 frag_pos;

uniform sampler2D texture1;

uniform vec3 light_color;
uniform vec3 light_pos;
uniform vec3 view_pos;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;

void main()
{
    vec3 ambient = vec3(0.1) * light.ambient * vec3(texture(material.diffuse, tex_coord));

    vec3 norm = normalize(normal);
    vec3 light_dir = normalize(light_pos - frag_pos);
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = (diff * vec3(texture(material.diffuse, tex_coord))) * light.diffuse;

    vec3 view_dir = normalize(view_pos - frag_pos);
    vec3 reflection = reflect(-light_dir, norm);
    float spec = pow(max(dot(view_dir, reflection), 0.0), material.shininess);
    vec3 specular = (vec3(texture(material.specular, tex_coord)) * spec) * light.specular;

    vec3 result = ambient + diffuse + specular;
    frag_color = vec4(result, 1.0);// * texture(texture1, tex_coord);
} 
