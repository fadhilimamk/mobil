#version 330 core
out vec4 FragColor;
in vec3 ourColor;
in vec2 textCoord;

uniform sampler2D ourTexture;
uniform float opacity;

void main() {
    vec3 alphaColor = ourColor * opacity;
    FragColor = texture(ourTexture, textCoord) * vec4(alphaColor, opacity);
}