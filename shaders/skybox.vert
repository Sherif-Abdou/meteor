#version 410 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTex;

layout(location = 0) out vec3 vTex;

uniform mat4 uPerspective;
uniform mat4 uModelView;

void main() {
    vec4 pos = uPerspective * mat4(mat3(uModelView)) * vec4(10*aPos, 1.0f);
    gl_Position = pos.xyww;
    vTex = aPos;
}
