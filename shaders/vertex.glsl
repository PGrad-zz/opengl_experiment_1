#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform int ground;

out VS_OUT {
    vec3 position;
    vec2 texCoords;
    vec3 normal;
} vs_out;

out vec2 alt_tex;

void main()
{
    vs_out.position = (view * vec4(aPos, 1.)).xyz;
    vs_out.texCoords = aTexCoords;
    alt_tex = aPos.xz / 100 + .5;
    vs_out.normal = (vec4(aNormal, 1.) * view).xyz;
    gl_Position = proj * vec4(vs_out.position, 1.);
}
