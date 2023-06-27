#version 410 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTex;

layout(location = 0) out vec3 vPos;
layout(location = 1) out vec3 vNormal;
layout(location = 2) out vec2 vTex;

void main() {
    gl_Position = vec4(aPos, 1.0f);
    vTex = aTex;
    vNormal = vec3(0.0f);
    vPos = vec3(0.0f);
}
