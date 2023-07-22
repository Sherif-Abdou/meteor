#version 410 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 aTangent;

layout(location = 0) out vec4 accum;
layout(location = 1) out float reveal;

void main() {
    vec4 color = vec4(0.0, 0.5, 0.0, 0.4f);
    float weight = max(min(1.0, max(max(color.r, color.g), color.b) * color.a), color.a) * clamp(0.03 / (1e-5 + pow(z / 200, 4.0)), 1e-2, 3e3);

    accum = vec4(color.rgb * color.a, color.a) * weight;

    reveal = color.a;
}