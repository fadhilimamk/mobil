#include <glad.h>
#include <glfw3.h>
#include <iostream>
#include <math.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "Shader.h"
#include "stb_image.h"
#include "Camera.h"

float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastX = 400, lastY = 300;
float fov = 45.0f;
bool firstMouse = true;
Camera cam = Camera();

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

int main() {
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

#pragma region tutorial_triangles
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

	float rectangle_vertices[] = {
		0.5f, 0.5f, 0.0f, 1, 0, 0, 1, 1,
		0.5f, -0.5f, 0.0f, 0, 1, 0, 1, 0,
		-0.5f, -0.5f, 0.0f, 0, 0, 1, 0, 0,
		-0.5f, 0.5f, 0.0f, 1, 1, 0, 0, 1
	};

	unsigned rectangle_indices[] = {
		0, 1, 3,
		1, 2, 3
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

	unsigned int triangle_vao;
	glGenVertexArrays(1, &triangle_vao);
	glBindVertexArray(triangle_vao);

	unsigned int triangle_vbo;
	glGenBuffers(1, &triangle_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, triangle_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW);

	unsigned int rectangle_ebo;
	glGenBuffers(1, &rectangle_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectangle_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectangle_indices), rectangle_indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	Shader triangle_shader("triangle_vShader.vs", "triangle_fShader.fs");

#pragma endregion tutorial_triangles

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// model matrix
		glm::mat4 triangle_model_mat = glm::mat4(1.0f);
		triangle_model_mat = glm::rotate(triangle_model_mat, (float)glfwGetTime() * glm::radians(-55.0f), glm::vec3(1, 0, 0));
		triangle_model_mat = glm::rotate(triangle_model_mat, (float)glfwGetTime() * glm::radians(-55.0f), glm::vec3(0, 1, 0));

		// view matrix
		glm::mat4 triangle_view_mat = glm::mat4(1.0f);
		triangle_view_mat = glm::lookAt(cam.Position, cam.Position + cam.Front, cam.Up);

		// projection matrix
		glm::mat4 triangle_pers_mat;
		triangle_pers_mat = glm::perspective(glm::radians(fov), 4.0f / 3.0f, 0.1f, 100.0f);

		triangle_shader.use();
		triangle_shader.setMat4("view", triangle_view_mat);
		triangle_shader.setMat4("perspective", triangle_pers_mat);

		glBindTexture(GL_TEXTURE_2D, triangle_tex);
		glBindVertexArray(triangle_vao);

		for (unsigned int i = 0; i < 10; i++) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1, 0.3f, 0.5f));
			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(45.0f), glm::vec3(1, 0.3f, 0.5f));

			triangle_shader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}