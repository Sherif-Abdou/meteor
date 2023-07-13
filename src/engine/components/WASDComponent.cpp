//
// Created by sheri on 7/10/2023.
//

#include "WASDComponent.h"

void WASDComponent::init() {

}

void WASDComponent::update(float deltaTime) {

}

const char *WASDComponent::getComponentName() {
    return "wasd_component";
}

void WASDComponent::physics_update(float deltaTime) {
    auto position = context.camera->getPosition();
    if (glfwGetKey(context.window, GLFW_KEY_W)) {
        position += sensitivity * deltaTime * entity.transform.getForward();
    }
    if (glfwGetKey(context.window, GLFW_KEY_S)) {
        position -= sensitivity * deltaTime * entity.transform.getForward();
    }
    if (glfwGetKey(context.window, GLFW_KEY_D)) {
        position += sensitivity * deltaTime * entity.transform.getRight();
    }
    if (glfwGetKey(context.window, GLFW_KEY_A)) {
        position -= sensitivity * deltaTime * entity.transform.getRight();
    }
    if (glfwGetKey(context.window, GLFW_KEY_SPACE)) {
        position += sensitivity * deltaTime * entity.transform.getUp();
    }
    if (glfwGetKey(context.window, GLFW_KEY_LEFT_SHIFT)) {
        position -= sensitivity * deltaTime * entity.transform.getUp();
    }
    context.camera->setCameraPosition(position);

    auto rotation = context.camera->getTransform().getEulerAngles();

    if (glfwGetKey(context.window, GLFW_KEY_Q)) {
        rotation.y += -sensitivity * deltaTime * 30.0f;
    }
    if (glfwGetKey(context.window, GLFW_KEY_E)) {
        rotation.y -= -sensitivity * deltaTime * 30.0f;
    }

    context.camera->setCameraRotation(rotation);
}

WASDComponent::WASDComponent(Entity &entity, Component::Context &context) : Component(entity, context) {}

