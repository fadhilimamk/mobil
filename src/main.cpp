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
#include "smoke.hpp"

using namespace std;

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// screen settings
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;

// init camera
Camera camera(glm::vec3(0.0f, 10.0f, 15.0f));

// FPS counter variable
double lastTime;
int nbFrames;

// delta time
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int carType = 0;
int prevCarType = 0;
vector<Model> models;

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

    GLFWwindow* window = glfwCreateWindow( SCR_WIDTH, SCR_HEIGHT, "Grafika - Mobil Bagus", NULL, NULL);
    if(!window){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        glfwTerminate();
        exit(-1);
    }

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetScrollCallback(window, scroll_callback);

    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        exit(-1);
    }

    // init shaders
    Shader shader("src/vertex_shader.vs", "src/fragment_shader.fs");
    Shader floorShader("src/vertex_shader_floor.vs", "src/fragment_shader_floor.fs");
    Shader rainShader("src/vertex_shader_rain.vs", "src/fragment_shader_rain.fs");
    Shader smokeShader("src/vertex_shader_smoke.vs", "src/fragment_shader_smoke.fs");

    glShadeModel(GL_SMOOTH);
    glClearColor(0, 0, 0, 1.0);
    glClearDepth(1.0);

    // enable depth buffer
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

    // init model and particle
    models.push_back(Model("assets/van/kendo.obj"));
    models.push_back(Model("assets/car/car.obj"));
    models.push_back(Model("assets/natla/natla_car.obj"));

    Floor ground;
    Rain rainParticleSystem;

    std::vector<std::vector<Smoke>> smokeParticleSystem;

    std::vector<Smoke> vanSmokeSystem;
    vanSmokeSystem.push_back(Smoke(
        glm::vec3(-2.5f, 0.3f, -0.58f), // initial position
        glm::vec3(0.1f, 0.1f, 0.1f), //color
        50, //amount
        3, //life
        0.5f //scale
    ));

    vanSmokeSystem.push_back(Smoke(
        glm::vec3(-2.5f, 0.3f, -0.47f), // initial position
        glm::vec3(0.1f, 0.1f, 0.1f), //color
        50, //amount
        3, //life
        0.5f //scale
    ));

    vanSmokeSystem.push_back(Smoke(
        glm::vec3(-2.5f, 0.3f, 0.48f), // initial position
        glm::vec3(0.1f, 0.1f, 0.1f), //color
        50, //amount
        3, //life
        0.5f //scale
    ));

    vanSmokeSystem.push_back(Smoke(
        glm::vec3(-2.5f, 0.3f, 0.6f), // initial position
        glm::vec3(0.1f, 0.1f, 0.1f), //color
        50, //amount
        3, //life
        0.5f //scale
    ));
    smokeParticleSystem.push_back(vanSmokeSystem);

    std::vector<Smoke> carSmokeSystem;
    smokeParticleSystem.push_back(carSmokeSystem);

    std::vector<Smoke> busSmokeSystem;
    smokeParticleSystem.push_back(busSmokeSystem);

    // Prepare for FPS calculation
    nbFrames = 0;
    lastTime = glfwGetTime();

    glm::mat4 originalView = camera.getViewMatrix();

    while(!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        double currentTime = glfwGetTime();
        nbFrames++;
        if (currentTime - lastTime >= 1.0) {
            printf("%d fps | %f ms/frame\n", nbFrames, 1000.0/double(nbFrames));
            nbFrames = 0;
            lastTime += 1.0;
        }

        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        rainParticleSystem.Update();

        for (int i = 0; i < smokeParticleSystem[carType].size(); i++) {
            smokeParticleSystem[carType][i].Update(deltaTime);
        }

        // set view matrix
        glm::mat4 view = camera.getViewMatrix();

        // get ratio
        GLint vp[4];
        glGetIntegerv(GL_VIEWPORT, vp);
        float ratio = vp[2] * 1.0 / vp[3];

        // set projection matrix
        glm::mat4 projection = glm::perspective(glm::radians(camera.getFOV()), ratio, 0.1f, 100.0f);

        // set model matrix
        glm::vec3 carPosition(0.0f, 0.0f, 0.0f);
        glm::mat4 model = glm::translate(glm::mat4(), carPosition);

        shader.use();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        shader.setMat4("model", model);

        // lighting
        glm::vec3 lightSource = glm::vec3(sin((float)glfwGetTime() * 20.0f), 5.0f, 10.0f);
        
        shader.setInt("material.specular", 50);
        shader.setFloat("material.shiness", 0.0f);
        shader.setVec3("light.position", lightSource);
        shader.setVec3("viewPos", camera.Position);
        shader.setVec3("light.ambient", 0.5f, 0.5f, 0.5f);
        shader.setVec3("light.diffuse", 1.0f, 1.0f, 1.0f);
        shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        models[carType].Draw(shader);

        rainShader.use();
        rainShader.setMat4("projection", projection);
        rainShader.setMat4("view", view);
        rainShader.setMat4("model", model);
        rainParticleSystem.Render(rainShader);

        floorShader.use();
        floorShader.setMat4("projection", projection);
        floorShader.setMat4("view", view);

        floorShader.setVec3("light.position", lightSource);
        floorShader.setVec3("light.ambient", 0.5f, 0.5f, 0.5f);
        floorShader.setVec3("light.diffuse", 1.0f, 1.0f, 1.0f);
        floorShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

        ground.Render(floorShader);

        smokeShader.use();
        smokeShader.setMat4("projection", projection);
        smokeShader.setMat4("view", view);
        smokeShader.setMat4("model", model);
        for (int i = 0; i < smokeParticleSystem[carType].size(); i++) {
            smokeParticleSystem[carType][i].Render(smokeShader);
        }

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
    static bool left_pressed = false;
    static bool right_pressed = false;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (key == GLFW_KEY_LEFT)
        left_pressed = action == GLFW_PRESS ? 1 : (action == GLFW_RELEASE ? 0 : left_pressed);
    else if (key == GLFW_KEY_RIGHT)
        right_pressed = action == GLFW_PRESS ? 1 : (action == GLFW_RELEASE ? 0 : right_pressed);
    
    if (left_pressed || right_pressed)
        prevCarType = carType;
        
    if (left_pressed)
        carType++;
    else if (right_pressed)
        carType--;
    if (carType < 0)
        carType = models.size() - 1;
    carType = carType % models.size();
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.changeFOV(yoffset);
}

void processInput(GLFWwindow *window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.gotoLeft(0.5);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.gotoRight(0.5);
}