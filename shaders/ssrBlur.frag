#version 410 core

layout(location = 2) in vec2 vTexCoord;

uniform sampler2D ssao;

out vec4 color;

const int width = 4;
const int height = 4;

void main() {
    vec2 texelSize = 1.0 / vec2(textureSize(ssao, 0));
    vec4 result = vec4(0.0);
    for (int x = -width/2; x < width/2; x++) {
        for (int y = -height/2; y < height/2; y++) {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            result += texture(ssao, vTexCoord + offset);
        }
    }
    color = result/(width*height);
}
