#ifndef FLOOR_H
#define FLOOR_H

#include "shader.hpp"

static const GLfloat FLOOR_VERTICES[] = {
            -1, 1, 0, // top left
            -1, -1, 0, // bottom left
            1, 1, 0, // top right

            1, 1, 0, // top right
            1, -1, 0, // bottom right
            -1, -1, 0 // bottom left
        };

static const GLfloat FLOOR_COLOR[] = {
            1, 1, 1, // top left
            1, 1, 1, // bottom left
            1, 1, 1, // top right
            
            1, 1, 1, // top right
            1, 1, 1, // bottom right
            1, 1, 1 // bottom left
        };

static const GLfloat FLOOR_TEX_CORDS[] = {
    0, 1, // top left
    0, 0, // bottom left
    1, 1, // top right
    
    1, 1, // top right
    1, 0, // bottom right
    0, 0 // bottom left
};

class Floor {
    public:
    Floor(GLfloat floorLevel = 0.0f) {
        this->floorLevel = floorLevel;
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        
        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(FLOOR_VERTICES) + sizeof(FLOOR_TEX_CORDS) + sizeof(FLOOR_COLOR), NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(FLOOR_VERTICES), FLOOR_VERTICES);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(FLOOR_VERTICES), sizeof(FLOOR_COLOR), FLOOR_COLOR);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(FLOOR_VERTICES) + sizeof(FLOOR_COLOR), sizeof(FLOOR_TEX_CORDS), FLOOR_TEX_CORDS);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(sizeof(FLOOR_VERTICES)));
        glEnableVertexAttribArray(1);

        // texture attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(sizeof(FLOOR_VERTICES) + sizeof(FLOOR_COLOR)));
        glEnableVertexAttribArray(2);

        glGenTextures(1, &texture_id);
        glBindTexture(GL_TEXTURE_2D, texture_id);

        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        
        unsigned char *woodTex = stbi_load("assets/ground.jpg", &tex_width, &tex_height, &tex_nrChannels, 0);
        if (woodTex) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, woodTex);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(woodTex);
        glBindTexture(GL_TEXTURE_2D, 0);

        glBindVertexArray(0);
    }

    void Render(Shader shader) {
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glBindVertexArray(vao);
        glm::mat4 model;
        model = glm::translate(model, glm::vec3(0.0f, floorLevel, 0.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1, 0, 0));
        model = glm::scale(model, glm::vec3(10));
        shader.setMat4("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    private:
    GLuint vao;
    GLuint vbo;
    GLuint texture_id;
    GLfloat floorLevel;
    int tex_width, tex_height, tex_nrChannels;
};

#endif