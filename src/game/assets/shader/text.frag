#version 450

layout(location = 0) in vec2 tex_coords;

layout(binding=0) uniform frag_ubo {
    vec3 uColor;
    float padding;
};

layout(binding=0) uniform sampler2D tex0;

layout(location = 0) out vec4 fragColor;

void main() {
    // vec4 obj_color = texture(tex0, tex_coords);
    fragColor = vec4(1.0, 0.0, 0.0, 1.0);
}