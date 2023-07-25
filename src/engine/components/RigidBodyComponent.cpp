//
// Created by sherif on 7/22/2023.
//

#include <iostream>
#include "RigidBodyComponent.h"

const glm::vec3 &RigidBodyComponent::getVelocity() const {
    return velocity;
}

void RigidBodyComponent::setVelocity(const glm::vec3 &velocity) {
    RigidBodyComponent::velocity = velocity;
}

const glm::vec3 &RigidBodyComponent::getAcceleration() const {
    return acceleration;
}

void RigidBodyComponent::setAcceleration(const glm::vec3 &acceleration) {
    RigidBodyComponent::acceleration = acceleration;
}

float RigidBodyComponent::getMass() const {
    return mass;
}

void RigidBodyComponent::setMass(float mass) {
    RigidBodyComponent::mass = mass;
}

void RigidBodyComponent::applyForce(const glm::vec3 & force) {
    acceleration += force * glm::vec3(getMass());
}

void RigidBodyComponent::applyImpulse(const glm::vec3 & impulse) {
    velocity += impulse * glm::vec3(getMass());
}

const char *RigidBodyComponent::getComponentName() {
    return component_name;
}

const char *RigidBodyComponent::getClassComponentName() {
    return component_name;
}

void RigidBodyComponent::init() {

}

void RigidBodyComponent::update(float deltaTime) {
}

RigidBodyComponent::RigidBodyComponent(Entity &entity, Component::Context &context) : Component(entity, context) {}

void RigidBodyComponent::physics_update(float deltaTime) {
    setVelocity(getVelocity() + glm::vec3(deltaTime) * getAcceleration());
    entity.transform.setPosition(entity.transform.getPosition() + glm::vec3(deltaTime) * getVelocity());
}

glm::vec3 RigidBodyComponent::getPosition() {
    return entity.transform.getPosition();
}

