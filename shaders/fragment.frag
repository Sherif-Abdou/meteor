#version 410 core
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vTexCoord;
layout(location = 3) in vec4 vLightSpaceCoord;

uniform vec3 uEyePosition;

uniform sampler2D shadowMap;

out vec4 color;
vec3 lightSource = vec3(0.0f, 2.0f, -0.0f);
vec3 eyeSource = vec3(0.0f, 0.0, 4.4f);

vec4 ambientColor = vec4(0.1f, 0.0f, 0.0f, 1.0f);
vec4 diffuseColor = vec4(0.9f, 0.0f, 0.0f, 1.0f);
vec4 specularColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
float ka = 0.3f;
float kd = 0.7f;
float ks = 0.4f;

float shadow_calculation(vec4 light_space_coord) {
    vec3 projCoord = light_space_coord.xyz / light_space_coord.w;
    projCoord = projCoord*0.5 + 0.5;

    float closestDepth = texture2D(shadowMap, projCoord.xy).r;
    float currentDepth = projCoord.z;

//    if (currentDepth <= closestDepth) {
//        discard;
//    }

    float bias = max(0.05 * (1.0 - vNormal.y), 0.005);
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
//    color = vec4(currentDepth - closestDepth);

    if (projCoord.z > 1.0) {
        shadow = 0.0;
    }
    return shadow;
}

void main() {
    vec3 delta = normalize(lightSource-vPos);
    float length = length(lightSource-vPos);
    float lambertian = max(dot(normalize(vNormal),delta),0.0f);
    vec3 reflected = reflect(-delta, normalize(vNormal));
    float specular = max(dot(normalize(uEyePosition-vPos), reflected), 0.0f);

    float attentuation = clamp(1.0/(0.5*pow(length, 2.0f)+1.0), 0.0, 1.0);
    float akd = kd * attentuation;
    float aks = ks * attentuation;

    float shadow = shadow_calculation(vLightSpaceCoord);

    color =
            ka * ambientColor +
            kd * (1.0 - shadow) * lambertian * diffuseColor +
            ks * (1.0 - shadow) * pow(specular, 256.0f) * specularColor;
//            + (1.0 - shadow) * vec4(0.0f, 0.9f, 0.0f, 1.0f);
//    color = mix(vec4(texture(shadowMap, vTexCoord)), vec4(1.0f, 0.0f, 0.0f, 1.0f), 0.0);
}