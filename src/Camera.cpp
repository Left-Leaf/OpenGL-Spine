#include <Camera.h>
#include <ResourceManager/ShaderManager.h>
#include <glm/ext/matrix_clip_space.hpp>


Camera::Camera(vec3 position, vec3 up, float yaw, float pitch)
    :Front(vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(MOUSESENSITIVITY), ScreenSensitivity(SCREENSENSITIVITY), Zoom(ZOOM)
{
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) 
    : Front(vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(MOUSESENSITIVITY), ScreenSensitivity(SCREENSENSITIVITY), Zoom(ZOOM)
{
    Position = vec3(posX, posY, posZ);
    WorldUp = vec3(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD) {
        float length = sqrt(Front.x * Front.x + Front.z * Front.z);
        Position.x += Front.x * velocity / length;
        Position.z += Front.z * velocity / length;
    }
    if (direction == BACKWARD) {
        float length = sqrt(Front.x * Front.x + Front.z * Front.z);
        Position.x -= Front.x * velocity / length;
        Position.z -= Front.z * velocity / length;
    }
    if (direction == LEFT) {
        float length = sqrt(Right.x * Right.x + Right.z * Right.z);
        Position.x -= Right.x * velocity / length;
        Position.z -= Right.z * velocity / length;
    }
    if (direction == RIGHT) {
        float length = sqrt(Right.x * Right.x + Right.z * Right.z);
        Position.x += Right.x * velocity / length;
        Position.z += Right.z * velocity / length;
    }
    if (direction == UP) {
        Position.y += velocity;
    }
    if (direction == DOWN) {
        Position.y -= velocity;
    }
    UpdataView();
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, float zoffset)
{
    this->Position.x -= xoffset * ScreenSensitivity;
    this->Position.y += yoffset * ScreenSensitivity;
    this->Position.z += zoffset * ScreenSensitivity;
    UpdataView();
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
    UpdataView();
}

void Camera::ProcessMouseScroll(float yoffset)
{
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}

void Camera::Move(vec3 Position)
{
    this->Position = Position;
    UpdataView();
}

void Camera::UpdataView()
{
    glm::mat4 view = GetViewMatrix();
    for (auto iter : ShaderManager::Shaders) {
        iter.second.Use();
        iter.second.SetMatrix4("view", view);
    }
}

void Camera::updateCameraVectors()
{
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up = glm::normalize(glm::cross(Right, Front));
}


