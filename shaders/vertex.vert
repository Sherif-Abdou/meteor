#version 410 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

layout(location = 0) out vec3 vPos;
smooth layout(location = 1) out vec3 vNormal;
layout(location = 2) out vec2 vTexCoord;


uniform mat4 uModelView;
uniform mat4 uPerspective;
uniform vec3 uEyePosition;

void main() {
    gl_Position = uPerspective * uModelView * vec4(aPos, 1.0f);
    vPos = aPos;
    vNormal = aNormal;
    vTexCoord = aTexCoord;
}