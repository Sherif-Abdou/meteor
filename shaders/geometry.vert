#version 410 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 aTangent;

layout(location = 0) out vec3 vPos;
layout(location = 1) out vec3 vNormal;
layout(location = 2) out vec2 vTexCoord;
layout(location = 3) out vec3 vTangent;


uniform mat4 uModelView;
uniform mat4 uPerspective;
uniform vec3 uEyePosition;
uniform mat4 uTransform;
uniform mat4 uNormalTransform;
uniform mat4 uNormalView;
uniform mat4 uLightSpaceMatrix;

void main() {
    gl_Position = uPerspective * uModelView * uTransform * vec4(aPos, 1.0f);
    vPos = vec3(uModelView * uTransform * vec4(aPos, 1.0f));
    vNormal = vec3(uNormalView * uNormalTransform * vec4(aNormal, 1.0f));
    vTangent = vec3(uModelView * uTransform * vec4(aTangent, 1.0f));
    vTexCoord = aTexCoord;
}
