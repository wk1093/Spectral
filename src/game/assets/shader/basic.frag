#version 450

layout(location = 0) in vec3 vertex_pos;
layout(location = 1) in vec3 normal_out;
layout(location = 2) in vec2 tex_coords;

layout(location = 0) out vec4 fragColor;


layout(location=3) uniform vec3 uViewPos;
layout(location=4) uniform float uTime;
layout(location=5) uniform sampler2D tex0;


void main() {
    vec3 light_pos = vec3(2.0, 5.0, -4.0);
    vec3 light_color = vec3(1.0);
    vec3 ambient_color = vec3(0.3);

    vec3 light_dir = normalize(light_pos - vertex_pos);
    vec3 view_dir = normalize(uViewPos - vertex_pos);
    vec3 reflect_dir = reflect(-light_dir, normalize(normal_out));

    float diff = max(dot(normal_out, light_dir), 0.0);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32.0);

    vec4 obj_color = texture(tex0, tex_coords);

    vec3 diffuse = diff * light_color;
    vec3 specular = spec * light_color;
    vec3 result = (ambient_color + diffuse + specular) * obj_color.rgb;

    fragColor = vec4(result, obj_color.a);
}