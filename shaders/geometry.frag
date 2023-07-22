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
uniform sampler2D normalMap;
uniform int normalMapUsed;
uniform vec4 solidAlbedo;
uniform bool useSolidAlbedo;

void main() {
    gPos = vPos;
    gNormal = normalize(vNormal);
    gTangent = normalize(vTangent);
    gAlbedo = useSolidAlbedo ? solidAlbedo : vec4(texture(albedo, vTexCoord).rgb, 0.0f);
    if (normalMapUsed == 1) {
        vec3 map_normal = texture(normalMap, vTexCoord).rgb;
        vec3 B = cross(gNormal, gTangent);
        mat3 TBN = mat3(gTangent, B, gNormal);
        gNormal = TBN * (2.0f * map_normal - 1.0f);
    }
}