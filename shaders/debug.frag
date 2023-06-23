#version 410 core

layout (location = 2) in vec2 tex;

uniform sampler2D map;

out vec4 color;

void main() {
    color = vec4(texture2D(map, tex).r);
}
