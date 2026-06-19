#pragma once

#include <glm/glm.hpp>

class Camera {
public:
    Camera();

    glm::vec3 get_position() const;
    glm::vec3 get_direction() const;
    glm::vec3 get_up() const;

    void mouse_update(float yaw, float pitch);

    enum struct CameraDirection {
        right, left, front, back, up, down
    };

    void push(glm::vec3 towards, float force=1.0f);
    void push(CameraDirection direction);

private:
    float camera_speed = 2.5f;
    glm::vec3 position;
    glm::vec3 direction;

    glm::vec3 right;
    glm::vec3 up;
};
