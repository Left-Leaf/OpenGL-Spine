#ifndef _CAMERA_H
#define _CAMERA_H

#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN,
};

const float YAW = -90.0f;
const float PITCH = -89.0f;
const float SPEED = 2.5f;
const float MOUSESENSITIVITY = 0.1f;
const float SCREENSENSITIVITY = 0.01f;
const float ZOOM = 45.0f;

using vec3 = glm::vec3;

class Camera
{
public:
    // camera Attributes
    vec3 Position;
    vec3 Front;
    vec3 Up;
    vec3 Right;
    vec3 WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float ScreenSensitivity;
    float Zoom;

    Camera(vec3 position = vec3(0.0f, 0.0f, 0.0f), vec3 up = vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    glm::mat4 GetViewMatrix();
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset, float zoffset);
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    void ProcessMouseScroll(float yoffset);
    void Move(vec3 Position);
    void UpdataView();
private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();
};

#endif // _CAMERA_H



