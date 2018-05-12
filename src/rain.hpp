#ifndef RAIN_H
#define RAIN_H

#include <glm/glm.hpp>
#include <vector>

#include "shader.hpp"

static const int DEFAULT_RAIN_PARTICLE = 10000;
static const GLfloat RAIN_VERTICES[] = {
            -0.05f, 0.0f, 0.0f, 0.0f, 0.3f, 
            0.0f, 0.6f, 0.0f, 0.5f, 1.0f,
            0.05f, 0.0f, 0.0f, 1.0f, 0.3f,
            -0.05f, 0.0f, 0.0f, 0.0f, 0.3f,
            0.05f, 0.0f, 0.0f, 1.0f, 0.3f,
            0.0f, -0.2f, 0.0f, 0.5f, 0.0f,
        };

struct RainParticle {
    glm::vec3 position;
    float life; // life < 0 = dead
};

class Rain {
    public:
    Rain(int amount = DEFAULT_RAIN_PARTICLE, GLfloat floorLevel = -1.0f) {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        
        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(RAIN_VERTICES), RAIN_VERTICES, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // load and create a texture 
        // -------------------------
        unsigned int texture;
        
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load image, create texture and generate mipmaps
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
        unsigned char *data = stbi_load("assets/rain.png", &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);

        this->floorLevel = floorLevel;

        for (int i = 0; i < amount; i++) {
            RainParticle particle;
            createRainParticle(&particle);
            particles.push_back(particle);
        }

    }

    void Update() {
        for (unsigned int i = 0; i < particles.size(); i++) {
            RainParticle* particle = &particles[i];
            if (particle->position.y < -10.0f && particle->life < 0.0f)
                createRainParticle(particle);
            else {
                particle->position.y -= 0.8f;
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