#version 410 core

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vTexCoord;
layout(location = 3) in vec3 vTangent;

layout(location = 0) out vec3 gPos;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec4 gAlbedo;
layout(location = 3) out vec3 gTangent;

uniform sampler2D albedo;

void main() {
    gPos = vPos;
    gNormal = normalize(vNormal);
    gTangent = normalize(vTangent);
    gAlbedo = vec4(texture(albedo, vTexCoord).rgb, 1.0f);
}