#version 410 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 uPerspective; // light space matrix
uniform mat4 uModelView;
uniform mat4 uTransform;

void main() {
    gl_Position = uPerspective * uModelView * uTransform * vec4(aPos, 1.0f);
//    gl_Position = vec4(0.25f, 0.25f, 1.0f, 1.0f);
//    gl_Position = vec4(0.1f, 0.1f, 0.4f, 1.0f);
}