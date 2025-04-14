#version 450

layout(location=0) in vec3 pos;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 texcoord;

layout(location=0) out vec3 vertex_pos;
layout(location=1) out vec3 normal_out;
layout(location=2) out vec2 tex_coords;

layout(location=0) uniform mat4 uProj;
layout(location=1) uniform mat4 uView;
layout(location=2) uniform mat4 uModel;

void main() {
    /*spsl_position = mul(uProj, mul(uView, mul(uModel, vec4(pos, 1.0))));
    normal_out = normalize(mul(convm4tom3(transpose(inverse(uModel))), normal));
    tex_coords = texcoord;
    vertex_pos = mul(uModel, vec4(pos, 1.0)).xyz;*/
    gl_Position = uProj * (uView * (uModel * vec4(pos, 1.0)));
    normal_out = normalize(mat3(transpose(inverse(uModel))) * normal);
    tex_coords = texcoord;
    vertex_pos = (uModel * vec4(pos, 1.0)).xyz;
}
