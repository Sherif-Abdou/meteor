#version 410 core

layout(location = 0) in vec3 vTex;

uniform samplerCube skybox;

out vec4 color;

void main() {
    color = vec4(texture(skybox, vTex));
//    color = vec4(1.0, 0.0, 0.0, 1.0);
}