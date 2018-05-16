#ifndef SMOKE_H
#define SMOKE_H

#include <glm/glm.hpp>
#include <vector>
#include <cstdlib>
#include <time.h>
#include <math.h>

#include "shader.hpp"

static const int DEFAULT_SMOKE_PARTICLE = 10000;
static const GLfloat SMOKE_VERTICES[] = {
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f
        };

struct SmokeParticle {
    glm::vec3 position;
	glm::vec3 direction;
	GLfloat velocity;
	GLfloat scale;
	GLfloat dt;
	GLfloat life;
};

class Smoke {
public:
	// Smoke(initialPos, color, amount, maxLife, scale, offset)
	Smoke(	glm::vec3 _initialPos = glm::vec3(0, 0, 0), 
			glm::vec3 _color = glm::vec3(0, 0, 0), 
			int amount = DEFAULT_SMOKE_PARTICLE,
			GLfloat _maxLife = 0.0f,
			GLfloat _maxScale = 1.0f)
	{
		glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        
        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(SMOKE_VERTICES), SMOKE_VERTICES, GL_DYNAMIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        initialPos = _initialPos;
        color = _color;
        maxLife = _maxLife;
        maxScale = _maxScale;

        for (int i = 0; i < amount; i++) {
            SmokeParticle particle;
            createSmokeParticle(&particle);
            particles.push_back(particle);
        }

        glBindVertexArray(0);
	}

	void Update(GLfloat deltaTime) {
        for (unsigned int i = 0; i < particles.size(); i++) {
            SmokeParticle* particle = &particles[i];

            if (particle->life >= 0) {
				// particle->direction += glm::vec3(0.0f, randRange(-0.02f, 0.02f), randRange(-0.02f, 0.02f));
            	particle->dt = particle->dt + deltaTime * 0.1;
				particle->life -= deltaTime;
            } else {
            	createSmokeParticle(particle);
            }
        }
    }

    void Render(Shader shader) {
        glBindVertexArray(vao);
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        for (unsigned int i = 0; i < particles.size(); i++) {
        	SmokeParticle* particle = &particles[i];

            glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, initialPos);
			model = glm::translate(model, particle->direction * particle->dt * particle->velocity);

			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(-55.0f), glm::vec3(1, 0, 0));
			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(-55.0f), glm::vec3(0, 1, 0));
			
			particle->scale = 0.04 + 0.3 * particle->dt;
			GLfloat currentScale = particle->scale;
			model = glm::scale(model, glm::vec3(currentScale, currentScale, currentScale));
			
			// shader.setFloat("opacity", 0.2f - particle->dt * 5);
			shader.setFloat("opacity", particle->life);
            
			shader.setMat4("model", model);
            shader.setVec3("color", color);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBindVertexArray(0);
    }

private:
    GLuint vao;
    GLuint vbo;
	glm::vec3 initialPos;
    glm::vec3 color;
	std::vector<SmokeParticle> particles;
	GLfloat maxLife;
	GLfloat maxScale;
	GLfloat offset;

	float randRange(float LO, float HI) {
		return (LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO))));
	}
	void createSmokeParticle(SmokeParticle* particle) {
        particle->direction = glm::vec3(-1.0f, randRange(-0.1f, 0.1f), randRange(-0.1f, 0.1f));
        particle->velocity = randRange(7, 10);
		particle->position = initialPos;
		particle->life = randRange(maxLife/4, maxLife/2);
		particle->scale = 1;
		particle->dt = 0;
    }
};

#endif