#version 410 core

layout(location = 2) in vec2 vTexCoord;

uniform sampler2D position;
uniform sampler2D normal;
uniform sampler2D albedo;

out vec4 color;
uniform mat4 uPerspective;

const float max_distance = 8.0f;
const float min_distance = 0.25f;
const int max_steps = 128;
const float thickness = 0.05f;

void main() {
    // Position and Normal in view space
    vec2 texSize = textureSize(position, 0).xy;
    vec3 fragPos = texture(position, vTexCoord).xyz;
    vec3 fragNormal = normalize(texture(normal, vTexCoord).xyz);
    float fragShinyness = texture(albedo, vTexCoord).a;
    if (fragNormal == vec3(0.0) || fragShinyness < 0.9) {
        discard;
    }

    vec3 incomingDirection = normalize(fragPos);
    vec3 reflection = normalize(reflect(incomingDirection, fragNormal));
    vec4 startPosition = vec4(fragPos + reflection * 0, 1.0);
    vec4 endPosition = vec4(fragPos + reflection * max_distance, 1.0);

    float currentDistance = min_distance;

    for (int i = 0; i < max_steps; i++) {
        vec4 currentPosition = vec4(fragPos + reflection * currentDistance, 1.0);
        vec4 screenPosition = uPerspective * currentPosition;
        screenPosition /= screenPosition.w;
        screenPosition.xy = screenPosition.xy * 0.5 + 0.5;
        if (screenPosition.x < 0.0 || screenPosition.x > 1.0) {
            discard;
        }
        if (screenPosition.y < 0.0 || screenPosition.y > 1.0) {
            discard;
        }
        float screenDepth = texture(position, screenPosition.xy).z;

        float diff = abs(currentPosition.z - screenDepth);
        if (diff > 0 && diff < thickness) {
            color = vec4(texture(albedo, screenPosition.xy).rgb, 1.0);
            return;
        }
        if (currentDistance > max_distance) {
            discard;
        }

        currentDistance += max_distance / max_steps;
    }
}
