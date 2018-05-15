#include "Shader.h"

char* Shader::readFile(const char* filename) {
	FILE* fp;
	errno_t err = fopen_s(&fp, filename, "r");

	if (err) {
		printf("Error opening "); printf(filename); printf("\n");
	}

	fseek(fp, 0, SEEK_END);

	long file_length = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	char *contents = new char[file_length + 1];

	for (int i = 0; i < file_length + 1; i++) {
		contents[i] = '\0';
	}

	fread(contents, 1, file_length, fp);

	contents[file_length + 1] = '\0';
	fclose(fp);

	return contents;
}

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {
	const char* vShaderCode = Shader::readFile(vertexPath);
	const char* fShaderCode = Shader::readFile(fragmentPath);

	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	// Compile vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::";
		std::cout << "VERTEX::";
		std::cout << "COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Compile fragment shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::";
		std::cout << "SHADER::";
		std::cout << "COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Create and link shader program
	id = glCreateProgram();
	glAttachShader(id, vertex);
	glAttachShader(id, fragment);
	glLinkProgram(id);

	glGetShaderiv(id, GL_LINK_STATUS, &success);

	if (!success) {
		glGetProgramInfoLog(id, 512, NULL, infoLog);
		std::cout << "ERROR::";
		std::cout << "SHADER_PROGRAM::";
		std::cout << "LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::use() {
	glUseProgram(id);
}

void Shader::setBool(const std::string &name, bool value) const {
	glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const {
	glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
	glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setVec2f(const std::string &name, glm::vec2 value) const {
	unsigned int loc = glGetUniformLocation(id, name.c_str());
	glUniform2f(loc, value.x, value.y);
}

void Shader::setVec3f(const std::string &name, glm::vec3 value) const {
	unsigned int loc = glGetUniformLocation(id, name.c_str());
	glUniform3f(loc, value.x, value.y, value.z);
}

void Shader::SetVec4f(const std::string &name, glm::vec4 value) const {
	unsigned int loc = glGetUniformLocation(id, name.c_str());
	glUniform4f(loc, value.x, value.y, value.z, value.w);
}

void Shader::setMat4(const std::string &name, glm::mat4 matrix) const {
	unsigned int triangle_model_loc = glGetUniformLocation(id, name.c_str());
	glUniformMatrix4fv(triangle_model_loc, 1, GL_FALSE, glm::value_ptr(matrix));
}