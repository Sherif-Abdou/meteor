#version 410 core
layout(location = 2) in vec2 vTexCoord;

uniform vec3 uEyePosition;
uniform mat4 uLightSpaceMatrix;

uniform sampler2D position;
uniform sampler2D normal;
uniform sampler2D noiseMap;

out float color;
uniform float raw_samples[64*3];
uniform mat4 uPerspective;
uniform mat4 uModelView;

const vec2 noiseScale = vec2(1920.0/4.0, 1080.0/4.0);
const float radius = 0.5f;
const float bias = 0.025f;

void main() {
    vec3 fragPos = texture(position, vTexCoord).xyz;
    vec3 fragNormal = texture(normal, vTexCoord).xyz;
//    if (fragNormal == vec3(0.0)) {
//        discard;
//    }
    vec3 randomVec = texture(noiseMap, vTexCoord * noiseScale).xyz;
    vec3 tangent = normalize(randomVec - fragNormal * dot(randomVec, fragNormal));
    vec3 bitangent = cross(fragNormal, tangent);
    mat3 TBN = mat3(tangent, bitangent, fragNormal);

    float occlusion = 0.0;
    for (int i = 0; i < 64; i++) {
        vec3 raw_sample = vec3(raw_samples[3*i], raw_samples[3*i + 1], raw_samples[3*i + 2]);
        vec3 samplePosition = TBN * raw_sample;
        samplePosition = fragPos + samplePosition * radius;
        vec4 offset = vec4(samplePosition, 1.0f);
        offset = uPerspective * offset;
        offset.xyz /= offset.w;
        offset.xyz = offset.xyz*0.5 + 0.5;
        float sampleDepth = texture(position, offset.xy).z;
//        if (texture(normal, offset.xy).xyz == vec3(0.0)) {
//            continue;
//        }
        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
        occlusion += (sampleDepth >= samplePosition.z + bias ? 1.0 : 0.0) * rangeCheck ;
//        color = vec4(sampleDepth);
    }
    occlusion = 1.0 - (occlusion/64.0);
    color = occlusion;
//    color /= 64.0/**/;
}