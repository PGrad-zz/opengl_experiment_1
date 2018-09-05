#version 450 core
uniform sampler2D framebuf;
uniform sampler2D texture_diffuse1;
uniform vec3 lightPos;
uniform vec3 eye;
uniform mat4 view;
uniform int ground;

float saturate(float f) {
    return clamp(f, 0., 1.);
}

float diffuse(vec3 n, vec3 l) {
    return saturate(dot(n, l));
}

float blinn(vec3 n, vec3 l, vec3 v) {
    vec3 h = normalize(l + v);
    return pow(saturate(dot(n, h)), 5.);
}

in vec2 alt_tex;

in VS_OUT {
    vec3 position;
    vec2 texCoords;
    vec3 normal;
} fs_in;
out vec3 FragColor;

float sum(vec3 v) {
    return (v.x + v.y + v.z);
}

float avg(vec3 v) {
    return sum(v) / 3.;
}

void main() {
    vec3 l = normalize((view * vec4(lightPos, 1.)).xyz - fs_in.position);
    vec3 v = normalize(fs_in.position);
    float groundmult = ground == 0 ? 0. : 1.;
    vec3 texColor = 2. * texture(texture_diffuse1, fs_in.texCoords).rgb
                                   - groundmult * step(.01, avg(texture(framebuf, (1. - alt_tex) + vec2(-0.01, -.01)).rgb));
    FragColor = clamp(texColor, 0., 1.);
}
