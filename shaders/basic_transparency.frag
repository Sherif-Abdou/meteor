#version 410 core

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vTexCoord;
layout(location = 3) in vec3 vTangent;

uniform mat4 uModelView;
uniform mat4 uPerspective;

out vec4 color;

void main() {
    color = vec4(0.0, 0.4, 0.0, 0.7);
}