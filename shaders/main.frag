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

    sampler2D decal;
};
struct Light {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform Material material;
uniform Light light;

void main()
{
    float dist = length(light.direction - frag_pos);
    float att = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

    vec3 ambient = vec3(0.1) * light.ambient * vec3(texture(material.diffuse, tex_coord));

    vec3 norm = normalize(normal);
    vec3 light_dir = normalize(-light.direction);
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = (diff * vec3(texture(material.decal, tex_coord) + texture(material.diffuse, tex_coord))) * light.diffuse;

    vec3 view_dir = normalize(view_pos - frag_pos);
    vec3 reflection = reflect(-light_dir, norm);
    float spec = pow(max(dot(view_dir, reflection), 0.0), material.shininess);
    vec3 specular = (vec3(texture(material.specular, tex_coord)) * spec) * light.specular;

    ambient  *= att;
    diffuse  *= att;
    specular *= att;

    vec3 result = ambient + diffuse + specular;
    frag_color = vec4(result, 1.0);// * texture(texture1, tex_coord);
} 
