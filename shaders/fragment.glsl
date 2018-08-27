#version 450 core
uniform sampler2D texture_diffuse;
in GS_OUT {
    vec3 position;
    vec2 texCoords;
    vec3 normal;
} gs_out;
out vec4 FragColor;
void main() {
	FragColor = texture(texture_diffuse, gs_out.texCoords);
}
