#version 330 core
out vec4 spsl_fragcolor;
#define mul(a,b) ((a)*(b))
in vec3 obj_color;
in vec3 vertex_pos;
in vec3 normal_out;

uniform vec3 uColorMult;
uniform float uTime;

void main() {
    vec3 lightPos = vec3(0.0, 5.0, 1.0);
    vec3 lightDir = normalize(lightPos - vertex_pos);
    vec3 normal = normalize(normal_out);
    float lightIntensity = max(dot(normal, lightDir), 0.0);
    vec3 lightColor = vec3(1.0, 1.0, 1.0);
//    vec3 color = obj_color;
    vec3 color = vec3(1.0, 0.0, 0.0);
    spsl_fragcolor = vec4(color, 1.0);
}