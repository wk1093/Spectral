#version 450

layout(location=0) in vec2 pos;
layout(location=1) in vec2 uv;

layout(location=0) out vec2 tex_coords;

layout(binding=0) uniform vert_ubo {
    mat4 uProj;
    mat4 uView;
    mat4 uModel;
    float uZ;
};

void main() {
    gl_Position = (uModel * vec4(pos.x, pos.y, 0.0, 1.0));
    // gl_Position = vec4(pos, 0.0, 1.0);
    tex_coords = uv;
}
