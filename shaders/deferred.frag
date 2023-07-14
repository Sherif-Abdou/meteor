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
uniform sampler2D reflectionMap;

uniform mat4 uModelView;

out vec4 color;
const vec3 lightSource = vec3(0.0f, 2.0f, -0.0f);
vec3 eyeSource = uEyePosition;

const vec4 specularColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
const float ka = 0.3f;
const float kd = 0.7f;
const float ks = 0.3f;
const float max_reflection = 0.2f;

float shadow_calculation(vec4 light_space_coord) {
    vec3 projCoord = light_space_coord.xyz / light_space_coord.w;
    projCoord = projCoord*0.5 + 0.5;

    float closestDepth = texture2D(shadowMap, projCoord.xy).r;
    float currentDepth = projCoord.z;

    vec3 vNormal = texture(normal, vTexCoord).xyz;
    float bias = max(0.05 * (1.0 - vNormal.y), 0.005);
    float shadow = currentDepth - bias > closestDepth ? 0.8 : 0.0;

    if (projCoord.z > 1.0) {
        shadow = 0.0;
    }
    return shadow;
}

void main() {
    vec4 diffuseColor = vec4(texture(albedo, vTexCoord).xyz, 1.0f);
    vec3 vPos = texture(position, vTexCoord).xyz;
    vec3 vNormal = texture(normal, vTexCoord).xyz;

    if (vNormal == vec3(0.0)) {
        discard;
    }
    float ambientOcclusion = texture(occlusionMap, vTexCoord).r;
    vec4 ssrReflection = texture(reflectionMap, vTexCoord).rgba;
    diffuseColor.rgb = mix(diffuseColor.rgb, vec3(ssrReflection), min(ssrReflection.a, max_reflection));

    vec3 delta = normalize(lightSource-vPos);
    float length = length(lightSource-vPos);
    float lambertian = max(dot(normalize(vNormal),delta),0.0f);
    vec3 reflected = reflect(-delta, normalize(vNormal));
    float specular = max(dot(normalize(uEyePosition-vPos), reflected), 0.0f);

    float attentuation = clamp(1.0/(0.5*pow(length, 2.0f)+1.0), 0.0, 1.0);
    float akd = kd * attentuation;
    float aks = ks * attentuation;

    float shadow = shadow_calculation(uLightSpaceMatrix * inverse(uModelView) * vec4(vPos, 1.0f));

    color =
    ka * ambientOcclusion * diffuseColor +
    kd * (1.0 - shadow) * lambertian * diffuseColor +
    ks * (1.0 - shadow) * pow(specular, 256.0f) * specularColor;


    color.rgb = color.rgb / (color.rgb + vec3(1.0f));
}
