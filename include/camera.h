//
// Created by Dante Rendell on 2023-10-24.
//

#ifndef LEARN_OPENGL_CAMERA_H
#define LEARN_OPENGL_CAMERA_H

#include "settings.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum MoveDirection {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
};

class Camera {
public:
    float speed;
    float sensitivity;
    float fov;

    glm::mat4 view;
    glm::mat4 projection;
    glm::vec3 pos;
    glm::vec3 front;
    glm::vec3 up;

    bool firstMouse = true;
    float lastX = (float)800 / 2.0;
    float lastY = (float)600 / 2.0f;
    float pitch = 0.0f;
    float yaw = -90.0f;

public:
    Camera();
    Camera(glm::vec3 pos, glm::vec3 dir, glm::vec3 up, float fov);
    void move(MoveDirection dir, float speed);
    void update_view();
    void handle_mouse(float xPos, float yPos);
};


#endif //LEARN_OPENGL_CAMERA_H
