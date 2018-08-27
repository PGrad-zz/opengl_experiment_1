#version 450 core
layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

uniform mat4 proj;
uniform float disp;

in VS_OUT {
    vec3 position;
    vec2 texCoords;
    vec3 normal;
} gs_in[];

out GS_OUT {
    vec3 position;
    vec2 texCoords;
    vec3 normal;
} gs_out;

void main() {
    for(int i = 0; i < 3; ++i) {
      gs_out.position = gs_in[i].position + gs_in[i].normal * disp;
      gl_Position = proj * vec4(gs_out.position, 1.);
      gs_out.texCoords = gs_in[i].texCoords;
      gs_out.normal = gs_in[i].normal;
      EmitVertex();
    }
    EndPrimitive();
}
