//
// Created by sheri on 7/6/2023.
//

#include <iostream>
#include "Transform.h"

glm::mat4 Transform::updateTransformMatrix() {
    glm::mat4 transform = calculateTransformMatrix();
    m.lock();
    Transform::transform = transform;
    m.unlock();

    return transform;
}

glm::mat4 Transform::calculateTransformMatrix() {
    glm::mat4 transform = getRotationMatrix();
    m.lock();
    transform = glm::translate(transform, position);
    transform = glm::scale(transform, scale);
    m.unlock();

    return transform;
}

glm::mat4 Transform::getRotationMatrix() {
    auto local_transform = glm::identity<glm::mat4>();
    if (this->parent != nullptr) {
        local_transform = this->parent->calculateTransformMatrix();
    }
    m.lock();
    local_transform = glm::rotate(local_transform, glm::radians(eulerAngles.x), glm::vec3(1.0, 0.0, 0.0));
    local_transform = glm::rotate(local_transform, glm::radians(eulerAngles.y), glm::vec3(0.0, 1.0, 0.0));
    local_transform = glm::rotate(local_transform, glm::radians(eulerAngles.z), glm::vec3(0.0, 0.0, 1.0));
    m.unlock();
    return local_transform;
}

glm::vec3 Transform::getPosition() const {
    return position;
}

void Transform::setPosition(const glm::vec3 &position) {
    m.lock();
    Transform::position = position;
    m.unlock();
    updateTransformMatrix();
}

glm::vec3 Transform::getEulerAngles() const {
    return eulerAngles;
}

void Transform::setEulerAngles(const glm::vec3 &eulerAngles) {
    m.lock();
    Transform::eulerAngles = eulerAngles;
    m.unlock();
    updateTransformMatrix();
}

glm::vec3 Transform::getScale() const {
    return scale;
}

void Transform::setScale(const glm::vec3 &scale) {
    m.lock();
    Transform::scale = scale;
    m.unlock();
    updateTransformMatrix();
}

const glm::mat4 &Transform::getTransform() const {
    return transform;
}

glm::vec3 Transform::getRight() {
    glm::vec4 value = getRotationMatrix() * glm::vec4(right, 1.0f);
    value /= glm::vec4(value.w);
    return glm::normalize(glm::vec3 {value});
}

glm::vec3 Transform::getUp() {
    glm::vec4 value = getRotationMatrix() * glm::vec4(up, 1.0f);
    value /= glm::vec4(value.w);
    return glm::normalize(glm::vec3 {value});
}

glm::vec3 Transform::getForward() {
    glm::vec4 value = getRotationMatrix() * glm::vec4(forward, 1.0f);
    value /= glm::vec4(value.w);
    return glm::normalize(glm::vec3 {value});
}

Transform *Transform::getParent() const {
    return parent;
}

void Transform::setParent(Transform *parent) {
    m.lock();
    Transform::parent = parent;
    m.unlock();
}
