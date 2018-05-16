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
        fov = 45.0f;
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

    void changeFOV(float yoffset) {
        if (fov >= 1.0f && fov <= 45.0f)
            fov -= yoffset;
        if (fov <= 1.0f)
            fov = 1.0f;
        if (fov >= 45.0f)
            fov = 45.0f;
    }

    float getZoomOut() {
        return ZoomOut;
    }

    float getFOV() {
        return fov;
    }

    private:
    float fov;
    float ZoomOut;
    float R;
    glm::vec3 Direction;
    glm::vec3 Right;

};

#endif