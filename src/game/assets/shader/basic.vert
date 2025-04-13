#version 450

layout(location=0) in vec3 pos;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 texcoord;

layout(location = 0) out vec3 vertex_pos;
layout(location = 1) out vec3 normal_out;
layout(location = 2) out vec2 tex_coords;

layout(set = 0, binding = 0) uniform ubo {
    mat4 uProj;
    mat4 uView;
    mat4 uModel;
};

void main() {
    vec4 world_pos = uModel * vec4(pos, 1.0);
    gl_Position = uProj * uView * world_pos;
    vertex_pos = world_pos.xyz;

    mat3 normal_matrix = transpose(inverse(mat3(uModel)));
    normal_out = normalize(normal_matrix * normal);

    tex_coords = texcoord;
}
