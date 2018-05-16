#version 330 core
out vec4 FragColor;

in vec3 Color;

uniform float opacity;

void main() {    
	vec3 alphaColor = Color * opacity;
    FragColor = vec4(alphaColor, opacity);
}