#ifndef FLOOR_H
#define FLOOR_H

#include "shader.hpp"

static const GLfloat FLOOR_VERTICES[] = {
            -20.0f, 0.0f, -20.0f,0.0, 1.0, 0.0, 
            -20.0f, 0.0f, 20.0f, 0.0, 1.0, 0.0, 
            20.0f, 0.0f, 20.0f, 0.0, 1.0, 0.0, 
            -20.0f, 0.0f, -20.0f, 0.0, 1.0, 0.0, 
            20.0f, 0.0f, 20.0f, 0.0, 1.0, 0.0, 
            20.0f, 0.0f, -20.0f, 0.0, 1.0, 0.0
        };

class Floor {
    public:
    Floor(GLfloat floorLevel = 0.0f) {
        this->floorLevel = floorLevel;
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        
        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(FLOOR_VERTICES), FLOOR_VERTICES, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    void Render(Shader shader) {
        glBindVertexArray(vao);
        glm::mat4 model;
        model = glm::translate(model, glm::vec3(0.0f, floorLevel, 0.0f));
        // shader.setMat4("model", model);
        shader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }

    private:
    GLuint vao;
    GLuint vbo;
    GLfloat floorLevel;
};

#endif