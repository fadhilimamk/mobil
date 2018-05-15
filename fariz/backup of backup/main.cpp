#include <glad.h>
#include <glfw3.h>
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <math.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Shader.h"
#include "stb_image.h"
#include "Camera.h"
#include "rain.hpp"

float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastX = 400, lastY = 300;
float fov = 45.0f;
bool firstMouse = true;
Camera cam = Camera();

float randRange(float LO, float HI) {
	return (LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO))));
}

void frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	cam.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	cam.ProcessMouseScroll(yoffset);
}

void processInput(GLFWwindow *window) {
	float cameraSpeed = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cam.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cam.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cam.ProcessKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cam.ProcessKeyboard(RIGHT, deltaTime);
	}
}

struct Particle {
	GLfloat scale;
	glm::vec3 position;
	glm::vec4 color;
	GLfloat currentLife;
	glm::vec3 startPos;
	glm::vec3 velocity;
	GLfloat travelled;
	GLfloat dt;
	GLfloat offset;

	Particle(glm::vec3 _startPos, glm::vec4 _color, glm::vec3 _velocity, GLfloat _scale, GLfloat _currentLife) {
		startPos = _startPos;
		position = startPos;
		color = _color;
		scale = _scale;
		currentLife = _currentLife;
		velocity = _velocity;
		travelled = 0;
		dt= 0;
		offset = 0;
	}

	Particle() {
		color = (glm::vec4(1));
		scale = (1);
		currentLife = (0);
		startPos = (glm::vec3(0));
		velocity = (glm::vec3(0));
		position = startPos;
		travelled = 0;
		dt = 0;
		offset = 0;
	}
};

int main() {
	srand(static_cast <unsigned> (time(0)));
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	int screenWidth = 800;
	int screenHeight = 600;

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpengGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Setup shaders
	Shader smoke_shader("test_particle_vector.vs", "test_particle_fragment.fs");
	Shader rainShader("vertex_shader_rain.vs", "fragment_shader_rain.fs");

	glShadeModel(GL_SMOOTH);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0);

	// Init model
	Rain rainParticleSystem;
	float triangle_vertices[] = {
		-0.5f, -0.5f, -0.5f,  0, 0, 0, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  0, 0, 0, 1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  0, 0, 0, 1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0, 0, 0, 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0, 0, 0, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0, 0, 0, 0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0, 0, 0, 0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0, 0, 0, 1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0, 0, 0, 1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0, 0, 0, 1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f, 0, 0, 0, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0, 0, 0, 0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  0, 0, 0, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0, 0, 0, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0, 0, 0, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0, 0, 0,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0, 0, 0, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0, 0, 0, 1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  0, 0, 0, 1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  0, 0, 0, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0, 0, 0, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0, 0, 0, 0.0f, 1.0f,
		0.5f, -0.5f,  0.5f, 0, 0, 0,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0, 0, 0, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0, 0, 0, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0, 0, 0,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f, 0, 0, 0, 1.0f, 0.0f,
		0.5f, -0.5f,  0.5f, 0, 0, 0, 1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, 0, 0, 0, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0, 0, 0, 0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,0, 0, 0,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,0, 0, 0,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,0, 0, 0,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f, 0, 0, 0, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,0, 0, 0,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,0, 0, 0,  0.0f, 1.0f
	};

	unsigned int triangle_tex;
	glGenTextures(1, &triangle_tex);
	glBindTexture(GL_TEXTURE_2D, triangle_tex);

	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int tex_width, tex_height, tex_nrChannels;
	unsigned char *woodTex = stbi_load("container.jpg", &tex_width, &tex_height, &tex_nrChannels, 0);
	if (woodTex) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, woodTex);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(woodTex);
	glBindTexture(GL_TEXTURE_2D, 0);

	// load cube data to gpu
	unsigned int triangle_vbo;
	glGenBuffers(1, &triangle_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, triangle_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW);
	
	// set smoke states
	GLuint smoke_vao;
	glGenVertexArrays(1, &smoke_vao);
	glBindVertexArray(smoke_vao);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	//glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// initialize smoke particles
	const int maxParticles = 100;
	std::vector<Particle> smokes;

	for (int i = 0; i < maxParticles; i++) {
		smokes.push_back(Particle(glm::vec3(randRange(-0.02, 0.02),0, randRange(-0.02, 0.02)), glm::vec4(0.5f, 0, 0, 1), glm::vec3(randRange(-1.5, 1.5), randRange(5, 10), randRange(-1.5, 1.5)), 0, 1.0f));
	}

	GLfloat scale = 0;
	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		rainParticleSystem.Update();

		// set model matrix
		glm::vec3 carPosition(0.0f, 0.0f, 0.0f);
		glm::mat4 xModel = glm::translate(glm::mat4(), carPosition);

		// view matrix
		glm::mat4 triangle_view_mat = glm::mat4(1.0f);
		triangle_view_mat = cam.GetViewMatrix();

		// projection matrix
		glm::mat4 triangle_pers_mat;
		triangle_pers_mat = glm::perspective(glm::radians(fov), 4.0f / 3.0f, 0.1f, 100.0f);

		rainShader.use();
		rainShader.setMat4("projection", triangle_pers_mat);
		rainShader.setMat4("view", triangle_view_mat);
		rainShader.setMat4("model", xModel);
		rainParticleSystem.Render(rainShader);

		smoke_shader.use();
		smoke_shader.setMat4("view", triangle_view_mat);
		smoke_shader.setMat4("perspective", triangle_pers_mat);

		glBindTexture(GL_TEXTURE_2D, triangle_tex);
		glBindVertexArray(smoke_vao);
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		for (int i = 0; i < maxParticles; i++) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(smokes[i].startPos));

			if (smokes[i].travelled < 1000) {
				smokes[i].dt = smokes[i].dt + deltaTime * 0.1;
				smokes[i].travelled += smokes[i].velocity.y;
			}
			else {
				smokes[i].velocity = glm::vec3(randRange(-1.5, 1.5), randRange(5, 10), randRange(-1.5, 1.5));
				smokes[i].startPos = glm::vec3(randRange(-0.02, 0.02), smokes[i].startPos.y, randRange(-0.02, 0.02));
				smokes[i].travelled = 0;
				smokes[i].dt = 0;
			}
			
			//scale = 0.01 + 0.5 * smokes[i].dt;
			smokes[i].scale = 0.01 + 0.5 * smokes[i].dt;
			model = glm::translate(model, smokes[i].velocity * (smokes[i].dt));
			
			smoke_shader.setFloat("opacity", 0.3f - smokes[i].dt * 5);
			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(-55.0f), glm::vec3(1, 0, 0));
			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(-55.0f), glm::vec3(0, 1, 0));
			//model = glm::scale(model, glm::vec3(scale, scale, scale));
			model = glm::scale(model, glm::vec3(smokes[i].scale, smokes[i].scale, smokes[i].scale));
			smoke_shader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}