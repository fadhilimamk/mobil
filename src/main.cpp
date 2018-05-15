#include <bits/stdc++.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "mesh.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include "model.hpp"
#include "floor.hpp"
#include "rain.hpp"
#include "particle.hpp"

using namespace std;

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

// screen settings
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;

// init camera
Camera camera(glm::vec3(0.0f, 5.0f, 15.0f));

// Global Variables
float deltaTime = 0.0f;
float lastFrame = 0.0f;

float randRange(float LO, float HI) {
	return (LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO))));
}

int main(int argc, char** argv) {
	
    // init window
    if (!glfwInit()) {
        fprintf(stderr, "failed to initialize glfw\n");
        exit(-1);
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow( SCR_WIDTH, SCR_HEIGHT, "Grafika-Car with lighting", NULL, NULL);
    if(!window){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        glfwTerminate();
        exit(-1);
    }

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwMakeContextCurrent(window);

    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        exit(-1);
    }

    // init shaders
    Shader shader("src/vertex_shader.vs", "src/fragment_shader.fs");
    Shader rainShader("src/vertex_shader_rain.vs", "src/fragment_shader_rain.fs");
	Shader smoke_shader("test_particle_vector.vs", "test_particle_fragment.fs");
	
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClearDepth(1.0);

    // enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // init model and particle
    Model car("assets/van/kendo.obj");
    Floor ground;
    Rain rainParticleSystem;

    while(!glfwWindowShouldClose(window)) {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.gotoRight(0.1f);
        rainParticleSystem.Update();

        // set view matrix
        glm::mat4 view = camera.getViewMatrix();

        // get ratio
        GLint vp[4];
        glGetIntegerv(GL_VIEWPORT, vp);
        float ratio = vp[2] * 1.0 / vp[3];

        // set projection matrix
        glm::mat4 projection = glm::perspective(glm::radians(30.0f), ratio, 0.1f, 100.0f);

        // set model matrix
        glm::vec3 carPosition(0.0f, 0.0f, 0.0f);
        glm::mat4 model = glm::translate(glm::mat4(), carPosition);

        shader.use();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        shader.setMat4("model", model);

        // lighting
        glm::vec3 lightSource = camera.Position + glm::vec3(0.0f, 10.0f, 0.0f);
        shader.setInt("material.specular", 100);
        shader.setFloat("material.shiness", 10.0f);
        shader.setVec3("light.position", lightSource);
        shader.setVec3("viewPos", camera.Position);
        shader.setVec3("light.ambient", 0.5f, 0.5f, 0.5f);
        shader.setVec3("light.diffuse", 1.0f, 1.0f, 1.0f);
        shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

        car.Draw(shader);

        rainShader.use();
        rainShader.setMat4("projection", projection);
        rainShader.setMat4("view", view);
        rainShader.setMat4("model", model);
        rainParticleSystem.Render(rainShader);
        // ground.Render(rainShader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    static bool a_pressed = false;
    static bool d_pressed = false;

    if (key == GLFW_KEY_A)
        a_pressed = action == GLFW_PRESS ? 1 : (action == GLFW_RELEASE ? 0 : a_pressed);
    else if (key == GLFW_KEY_S)
        d_pressed = action == GLFW_PRESS ? 1 : (action == GLFW_RELEASE ? 0 : d_pressed);
    
    if (a_pressed)
        camera.gotoLeft(0.5);
    else if (d_pressed)
        camera.gotoRight(0.5);

}