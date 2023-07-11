//
// Created by sheri on 7/6/2023.
//

#include "Transform.h"

glm::mat4 Transform::updateTransformMatrix() {
    glm::mat4 transform = getRotationMatrix();
    transform = glm::translate(transform, this->position);
    transform = glm::scale(transform, scale);

    this->transform = transform;
    return transform;
}

glm::mat4 Transform::getRotationMatrix() const {
    auto transform = glm::identity<glm::mat4>();
    transform = glm::rotate(transform, glm::radians(eulerAngles.x), glm::vec3(1.0, 0.0, 0.0));
    transform = glm::rotate(transform, glm::radians(eulerAngles.y), glm::vec3(0.0, 1.0, 0.0));
    transform = glm::rotate(transform, glm::radians(eulerAngles.z), glm::vec3(0.0, 0.0, 1.0));
    return transform;
}

const glm::vec3 &Transform::getPosition() const {
    return position;
}

void Transform::setPosition(const glm::vec3 &position) {
    Transform::position = position;
    updateTransformMatrix();
}

const glm::vec3 &Transform::getEulerAngles() const {
    return eulerAngles;
}

void Transform::setEulerAngles(const glm::vec3 &eulerAngles) {
    Transform::eulerAngles = eulerAngles;
    updateTransformMatrix();
}

const glm::vec3 &Transform::getScale() const {
    return scale;
}

void Transform::setScale(const glm::vec3 &scale) {
    Transform::scale = scale;
    updateTransformMatrix();
}

const glm::mat4 &Transform::getTransform() const {
    return transform;
}

glm::vec3 Transform::getRight() const {
    glm::vec4 value = getRotationMatrix() * glm::vec4(right, 1.0f);
    value /= glm::vec4(value.w);
    return glm::normalize(glm::vec3 {value});
}

glm::vec3 Transform::getUp() const {
    glm::vec4 value = getRotationMatrix() * glm::vec4(up, 1.0f);
    value /= glm::vec4(value.w);
    return glm::normalize(glm::vec3 {value});
}

glm::vec3 Transform::getForward() const {
    glm::vec4 value = getRotationMatrix() * glm::vec4(forward, 1.0f);
    value /= glm::vec4(value.w);
    return glm::normalize(glm::vec3 {value});
}
