#version 330 core

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 floorColors;
in vec2 texCords;
in vec3 FragPos;

out vec4 FragColor;

uniform sampler2D ourTexture;
uniform Light light;

void main() {
	vec3 Normal = vec3(0, 1, 0);

    vec3 ambient = light.ambient * texture(ourTexture, texCords).rgb;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(ourTexture, texCords).rgb;

    FragColor = vec4((ambient + diffuse), 1.0f);
}
