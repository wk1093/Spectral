#version 450

layout(location=0) in vec3 pos;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 texcoord;

layout(location=0) out vec3 vertex_pos;
layout(location=1) out vec3 normal_out;
layout(location=2) out vec2 tex_coords;

layout(binding=0) uniform vert_ubo {
    mat4 uProj;
    mat4 uView;
    mat4 uModel;
};

void main() {
    gl_Position = uProj * (uView * (uModel * vec4(pos, 1.0)));
    normal_out = normalize(mat3(transpose(inverse(uModel))) * normal);
    tex_coords = texcoord;
    vertex_pos = (uModel * vec4(pos, 1.0)).xyz;
}
