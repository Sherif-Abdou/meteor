//
// Created by sheri on 7/10/2023.
//

#include "WASDComponent.h"
#include "HitboxComponent.h"

void WASDComponent::init() {

}

void WASDComponent::update(float deltaTime) {

}

const char *WASDComponent::getComponentName() {
    return "wasd_component";
}

void WASDComponent::physics_update(float deltaTime) {
    auto position = context.camera->getPosition();
    auto& camera_transform = context.camera->getTransform();
    if (glfwGetKey(context.window, GLFW_KEY_W)) {
        position += sensitivity * deltaTime * camera_transform.getForward();
    }
    if (glfwGetKey(context.window, GLFW_KEY_S)) {
        position -= sensitivity * deltaTime * camera_transform.getForward();
    }
    if (glfwGetKey(context.window, GLFW_KEY_D)) {
        position += sensitivity * deltaTime * camera_transform.getRight();
    }
    if (glfwGetKey(context.window, GLFW_KEY_A)) {
        position -= sensitivity * deltaTime * camera_transform.getRight();
    }
    if (glfwGetKey(context.window, GLFW_KEY_SPACE)) {
        position += sensitivity * deltaTime * camera_transform.getUp();
    }
    if (glfwGetKey(context.window, GLFW_KEY_LEFT_SHIFT)) {
        position -= sensitivity * deltaTime * camera_transform.getUp();
    }
    context.camera->setCameraPosition(position);

    auto rotation = context.camera->getRotation();

    if (glfwGetKey(context.window, GLFW_KEY_Q)) {
        rotation.y += -sensitivity * deltaTime * 30.0f;
    }
    if (glfwGetKey(context.window, GLFW_KEY_E)) {
        rotation.y -= -sensitivity * deltaTime * 30.0f;
    }

    context.camera->setCameraRotation(rotation);

    if (entity.getComponent<HitboxComponent>().isColliding) {
        multipler = -1.0f;
    }

    auto current_position = entity.transform.getPosition();
    current_position.y -= 0.8f * multipler * deltaTime;
    entity.transform.setPosition(current_position);
}

WASDComponent::WASDComponent(Entity &entity, Component::Context &context) : Component(entity, context) {}

