//
// Created by Dante Rendell on 2023-10-24.
//

#include "camera.h"
#include "settings.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera() {
    fov = FOV;
    pos = glm::vec3(0.0f, 0.0f, 3.0f);
    front = glm::vec3(0.0f, 0.0f, -1.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    view = glm::lookAt(pos,
                       pos + front,
                       up);
}

Camera::Camera(glm::vec3 pos, glm::vec3 dir, glm::vec3 up, float fov) :pos(pos), front(dir), up(up), fov(fov) {
    view = glm::lookAt(pos,
                       pos + front,
                       up);
}

void Camera::move(MoveDirection dir, float speed) {
    if (dir == FORWARD)
        pos += speed * front;
    if (dir == BACKWARD)
        pos -= speed * front;
    if (dir == LEFT)
        pos -= glm::normalize(glm::cross(front, up)) * speed;
    if (dir == RIGHT)
        pos += glm::normalize(glm::cross(front, up)) * speed;
}

void Camera::update_view() {
    projection = glm::perspective(glm::radians(fov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
    view = glm::lookAt(pos,
                       pos + front,
                       up);
}

void Camera::handle_mouse(float xPos, float yPos) {
    if (firstMouse) {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    float xOffset = xPos - lastX;
    float yOffset = lastY - yPos;
    lastX = xPos;
    lastY = yPos;

    xOffset *= SENSITIVITY;
    yOffset *= SENSITIVITY;

    yaw += xOffset;
    pitch += yOffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(direction);
}


