#version 330 core
#define mul(a,b) ((a)*(b))
layout(location = 0) in  vec3 pos;
layout(location = 1) in  vec3 normal;
layout(location = 2) in  vec3 color;

out vec3 obj_color;
out vec3 vertex_pos;
out vec3 normal_out;

uniform mat4 uProj;
uniform mat4 uView;

void main() {
    gl_Position = vec4(pos, 1.0);
    vertex_pos = pos;
    obj_color = color;
    normal_out = normal;
}
