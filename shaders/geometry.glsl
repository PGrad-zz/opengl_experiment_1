#version 450 core
layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

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
      gl_Position = gl_in[i].gl_Position + vec4(gs_in[i].normal, 0);
      gs_out.position = gs_in[i].position + gs_in[i].normal;
      gs_out.texCoords = gs_in[i].texCoords;
      gs_out.normal = gs_in[i].normal;
      EmitVertex();
    }
    EndPrimitive();
}
