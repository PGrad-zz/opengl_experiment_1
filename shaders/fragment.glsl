#version 450 core
in vec2 texCoords;
out vec4 FragColor;
uniform sampler2D texture_diffuse;
uniform vec3 lightPos;
uniform vec3 eye;

float saturate(float f) {
    return clamp(f, 0., 1.);
}

float diffuse(vec3 n, vec3 l) {
    return saturate(dot(n, l));
}

float blinn(vec3 n, vec3 l, vec3 v) {
    vec3 h = normalize(l + v);
    return saturate(dot(n, h));
}

in VS_OUT {
    vec3 position;
    vec2 texCoords;
    vec3 normal;
} fs_in;

void main() {
    vec3 l = normalize(lightPos - fs_in.position);
    vec3 v = normalize(eye - fs_in.position);
    FragColor = texture(texture_diffuse, fs_in.texCoords) * (diffuse(fs_in.normal, l) + blinn(fs_in.normal, l, v) + .1);
}
