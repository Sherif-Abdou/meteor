#version 410 core
layout(location = 2) in vec2 vTexCoord;

uniform vec3 uEyePosition;
uniform mat4 uLightSpaceMatrix;

uniform sampler2D position;
uniform sampler2D normal;
uniform sampler2D albedo;
uniform sampler2D tangent;
uniform sampler2D shadowMap;
uniform sampler2D occlusionMap;

out vec4 color;
vec3 lightSource = vec3(0.0f, 4.0f, -0.0f);
vec3 eyeSource = vec3(0.0f, 0.0, 2.0f);

vec4 specularColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
float ka = 0.3f;
float kd = 0.8f;
float ks = 0.8f;

float shadow_calculation(vec4 light_space_coord) {
    vec3 projCoord = light_space_coord.xyz / light_space_coord.w;
    projCoord = projCoord*0.5 + 0.5;

    float closestDepth = texture2D(shadowMap, projCoord.xy).r;
    float currentDepth = projCoord.z;

    vec3 vNormal = texture(normal, vTexCoord).xyz;
    float bias = max(0.05 * (1.0 - vNormal.y), 0.005);
//    float bias = 0.005;
    float shadow = currentDepth - bias > closestDepth ? 0.8 : 0.0;
    //    color = vec4(currentDepth - closestDepth);

    if (projCoord.z > 1.0) {
        shadow = 0.0;
    }
    return shadow;
}

void main() {
    vec4 diffuseColor = vec4(texture(albedo, vTexCoord).xyz, 1.0f);
    vec3 vPos = texture(position, vTexCoord).xyz;
    vec3 vNormal = texture(normal, vTexCoord).xyz;

    if (vNormal == vec3(0.0, 0.0, 0.0)) {
        discard;
    }
    float ambientOcclusion = texture(occlusionMap, vTexCoord).r;
//    ambientOcclusion = 1.0f;

    vec3 delta = normalize(lightSource-vPos);
    float length = length(lightSource-vPos);
    float lambertian = max(dot(normalize(vNormal),delta),0.0f);
    vec3 reflected = reflect(-delta, normalize(vNormal));
    float specular = max(dot(normalize(uEyePosition-vPos), reflected), 0.0f);

    float attentuation = clamp(1.0/(0.5*pow(length, 2.0f)+1.0), 0.0, 1.0);
    float akd = kd * attentuation;
    float aks = ks * attentuation;

    float shadow = shadow_calculation(uLightSpaceMatrix * vec4(vPos, 1.0f));
//    float shadow = 0.0;

    color =
    ka * ambientOcclusion * diffuseColor +
    kd * (1.0 - shadow) * lambertian * diffuseColor +
    ks * (1.0 - shadow) * pow(specular, 256.0f) * specularColor;

//    color = vec4(vec3(ambientOcclusion), 1.0f);

    color.rgb = color.rgb / (color.rgb + vec3(1.0f));
//    color.rgb = pow(color.rgb, vec3(1/2.2f));
}
