#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera {
    public:
    glm::vec3 Position;
    glm::vec3 Target;
    glm::vec3 Up;
    float Zoom;

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f)) {
        Position = position;
        Target = target;
        Up = up;
        
        Direction = glm::normalize(Target-Position);
        Right = glm::cross(Direction, Up);
        R = glm::distance(Target, Position);
        Zoom = 45.0f;
        MovementSpeed = 2.5f;
    }

    glm::mat4 GetViewMatrix() {
        return glm::lookAt(Position, Target, Up);
    }

    void ProcessKeyboard(CameraMovement direction, float deltaTime) {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            Position += Target * velocity;
        if (direction == BACKWARD)
            Position -= Target * velocity;
        if (direction == LEFT)
            Position -= Target * velocity;
        if (direction == RIGHT)
            Position += Target * velocity;
    }

    void gotoRight(float alpha) {
        Position += glm::normalize(Right) * alpha;
        Position = glm::normalize(Position) * R;
        Direction = glm::normalize(Target-Position);
        Right = glm::cross(Direction, Up);
    }

    void gotoLeft(float alpha) {
        Position -= glm::normalize(Right) * alpha;
        Position = glm::normalize(Position) * R;
        Direction = glm::normalize(Target-Position);
        Right = glm::cross(Direction, Up);
    }

    private:
    float MovementSpeed;
    float R;
    glm::vec3 Direction;
    glm::vec3 Right;

};

#endif