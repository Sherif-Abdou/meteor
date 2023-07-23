//
// Created by sherif on 7/15/2023.
//

#include "HitboxComponent.h"

void HitboxComponent::init() {

}

void HitboxComponent::update(float deltaTime) {

}

const char *HitboxComponent::getComponentName() {
    return component_name;
}

void HitboxComponent::updateHitbox() {
    auto translation = entity.transform.getPosition();
    hitbox.startX = translation.x + offset.x - width/2.0f;
    hitbox.endX = translation.x + offset.x + width/2.0f;
    hitbox.startY = translation.y + offset.y - height/2.0f;
    hitbox.endY = translation.y + offset.y + height/2.0f;
    hitbox.startZ = translation.z + offset.z - depth/2.0f;
    hitbox.endZ = translation.z + offset.z + depth/2.0f;
}

void HitboxComponent::physics_init() {
    updateHitbox();
}

void HitboxComponent::physics_update(float deltaTime) {
    updateHitbox();
}

AABB *HitboxComponent::getHitbox() {
    return &hitbox;
}

HitboxComponent::HitboxComponent(Entity &entity, Component::Context &context) : Component(entity, context) {}

unsigned int HitboxComponent::getHitboxId() {
    return getHitbox()->getId();
}

const char *HitboxComponent::getClassComponentName() {
    return component_name;
}
