#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

const float ZOOM_OUT = 80.0f;

class Camera {
    public:
    glm::vec3 Position;
    glm::vec3 Target;
    glm::vec3 Up;

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f)) {
        Position = position;
        Target = target;
        Up = up;
        
        Direction = glm::normalize(Target-Position);
        Right = glm::cross(Direction, Up);
        R = glm::distance(Target, Position);
        ZoomOut = ZOOM_OUT;
    }

    glm::mat4 getViewMatrix() {
        return glm::lookAt(Position, Target, Up);
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

    float getZoomOut() {
        return ZoomOut;
    }

    private:
    float ZoomOut;
    float R;
    glm::vec3 Direction;
    glm::vec3 Right;

};

#endif