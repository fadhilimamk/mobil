#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTextCoord;

out vec3 ourColor;
out vec2 textCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;

void main() {
	gl_Position = perspective * view * model * vec4(aPos, 1.0);
	//ourColor = aColor;
	ourColor = vec3(0.5f, 0.5f, 0.5f);
	textCoord = aTextCoord;
}