#include "camera.hpp"
#include "delta_time.hpp"

#include <glm/glm.hpp>

Camera::Camera() {
    position = glm::vec3(0,0, 3);
    direction = glm::vec3(0,0, -1.0f);

    right = glm::vec3(1.0f,0,0);
    up = glm::vec3(0,1.0f,0);
}

glm::vec3 Camera::get_position() const {
    return position;
}

glm::vec3 Camera::get_direction() const {
    return direction;
}
glm::vec3 Camera::get_up() const {
    return up;
}

void Camera::mouse_update(float yaw, float pitch) {
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction = glm::normalize(direction);

    right = glm::normalize(glm::cross(direction, glm::vec3(0,1.0f,0)));
    up = glm::normalize(glm::cross(right, direction));

}

void Camera::push(glm::vec3 towards, float force) {
    position += towards * force * DeltaTime::get(); // * deltatime
}

void Camera::push(CameraDirection cam_dir) {
    switch (cam_dir) {
        case Camera::CameraDirection::right:
        {
            position += right * camera_speed;
            break;
        }
        case Camera::CameraDirection::left:
        {
            position -= right * camera_speed;
            break;
        }
        case Camera::CameraDirection::front:
        {
            position += direction * camera_speed;
            break;
        }
        case Camera::CameraDirection::back:
        {
            position -= direction * camera_speed;
            break;
        }
        case Camera::CameraDirection::up:
        {
            position += up * camera_speed;
            break;
        }
        case Camera::CameraDirection::down:
        {
            position -= up * camera_speed;
            break;
        }
    }
}
