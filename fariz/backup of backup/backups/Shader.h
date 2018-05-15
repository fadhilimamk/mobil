#pragma once
#include <glad.h>
#include <iostream>
#include <string>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

class Shader {
public:
	GLuint id;

	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);

	void use();

	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	void setVec2f(const std::string &name, glm::vec2 value) const;
	void setVec3f(const std::string &name, glm::vec3 value) const;
	void SetVec4f(const std::string &name, glm::vec4 value) const;
	void setMat4(const std::string &name, glm::mat4 value) const;

	static char* readFile(const char* filename);
};