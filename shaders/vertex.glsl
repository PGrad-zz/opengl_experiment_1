#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec2 texCoords;

void main()
{
    texCoords = aTexCoords;
    gl_Position = proj * view * vec4(aPos, 1.);
}
