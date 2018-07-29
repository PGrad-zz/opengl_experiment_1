#version 450 core
uniform sampler2D texture_diffuse;
in vec2 texCoords;
out vec4 FragColor;
void main() {
	FragColor = texture(texture_diffuse, texCoords);
}
