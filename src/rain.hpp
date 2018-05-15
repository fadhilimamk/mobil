#ifndef RAIN_H
#define RAIN_H

#include <glm/glm.hpp>
#include <vector>

#include "shader.hpp"

static const int DEFAULT_RAIN_PARTICLE = 10000;
static const GLfloat RAIN_VERTICES[] = {
            -0.02, 0.0f, 0.0f,
            0.0f, 0.3f, 0.0f,
            0.02, 0.0f, 0.0f,
            -0.02, 0.0f, 0.0f,
            0.02, 0.0f, 0.0f,
            0.0f, -0.1f, 0.0f
        };

struct RainParticle {
    glm::vec3 position;
    float life; // life < 0 = dead
};

class Rain {
    public:
    Rain(int amount = DEFAULT_RAIN_PARTICLE, GLfloat floorLevel = 0.0f) {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        
        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(RAIN_VERTICES), RAIN_VERTICES, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        this->floorLevel = floorLevel;

        for (int i = 0; i < amount; i++) {
            RainParticle particle;
            createRainParticle(&particle);
            particles.push_back(particle);
        }

        glBindVertexArray(0);
    }

    void Update() {
        for (unsigned int i = 0; i < particles.size(); i++) {
            RainParticle* particle = &particles[i];
            if (particle->position.y < -10.0f && particle->life < 0.0f)
                createRainParticle(particle);
            else {
                particle->position.y -= 0.5f;
                particle->position.x -= 0.3f;
                particle->life -= 0.03f;
            }
        }
    }

    void Render(Shader shader) {
        glBindVertexArray(vao);
        for (unsigned int i = 0; i < particles.size(); i++) {
            glm::mat4 model;
            float angle = 20.0f;

            model = glm::translate(model, particles[i].position);
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            shader.setMat4("model", model);

            shader.setVec4("color", glm::vec4(0.439, 0.502, 0.565, 0.8));

            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
        glBindVertexArray(0);
    }

    private:
    GLuint vao;
    GLuint vbo;
    GLfloat floorLevel;
    std::vector<RainParticle> particles;

    void createRainParticle(RainParticle* particle) {
        float x = (float) (rand() % 10000) / 10000.0f * 200.0f - 100.0f;
        float z = (float) (rand() % 10000) / 10000.0f * 200.0f - 100.0f;
        float y = (float) (rand() % 10000) / 10000.0f * 50.0f + 50.0f;
        float life = (float) ((rand() + 5000) % 10000) / 10000.0f;

        particle->position = glm::vec3(x, y, z);
        particle->life = life;
    }
};

#endif