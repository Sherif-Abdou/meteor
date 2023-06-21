#version 410 core
layout(location = 0) in vec3 vPos;
smooth layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vTexCoord;

uniform vec3 uEyePosition;

out vec4 color;
vec3 lightSource = vec3(2.0f, 2.0f, 3.0f);
vec3 eyeSource = vec3(0.0f, 0.0, 4.4f);

vec4 ambientColor = vec4(0.3f, 0.0f, 0.0f, 1.0f);
vec4 diffuseColor = vec4(0.9f, 0.0f, 0.0f, 1.0f);
vec4 specularColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
float ka = 0.3f;
float kd = 0.7f;
float ks = 1.0f;

void main() {
    vec3 delta = normalize(lightSource-vPos);
    float lambertian = max(dot(normalize(vNormal),delta),0.0f);
    vec3 reflected = reflect(-delta, normalize(vNormal));
    float specular = max(dot(normalize(uEyePosition-vPos), reflected), 0.0f);
    color = ka * ambientColor + kd * lambertian * diffuseColor + ks * pow(specular, 256.0f) * specularColor;
}