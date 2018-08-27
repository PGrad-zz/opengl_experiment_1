#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out VS_OUT {
    vec3 position;
    vec2 texCoords;
		vec3 normal;
} vs_out;

void main()
{
    vs_out.texCoords = aTexCoords;
    vs_out.normal = aNormal;
    vs_out.position = (view * vec4(aPos, 1.)).xyz;
    gl_Position = proj * view * vec4(aPos, 1.);
}
