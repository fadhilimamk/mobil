#version 330 core
in vec3 floorColors;
in vec2 texCords;

out vec4 FragColor;

uniform sampler2D ourTexture;

void main() {
    FragColor = texture(ourTexture, texCords) * vec4(floorColors, 1.0f);
}
