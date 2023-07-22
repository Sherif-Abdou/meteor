#version 410 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 aTangent;

uniform sampler2D accumTexture;
uniform sampler2D revealTexture;

out vec4 color;

void main() {
    vec4 accum = texture2D(accumTexture, gl_FragCoord.xy);
    float reveal = texture2D(revealTexture, gl_FragCoord.xy).r;

    color = vec4(accum.rgb / max(accum.a, 1e-5), reveal);

}