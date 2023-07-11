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
    auto position = entity.transform.getPosition();
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
    entity.transform.setPosition(position);

    auto rotation = entity.transform.getEulerAngles();

    if (glfwGetKey(context.window, GLFW_KEY_Q)) {
        rotation.y += sensitivity * deltaTime * 30.0f;
    }
    if (glfwGetKey(context.window, GLFW_KEY_E)) {
        rotation.y -= sensitivity * deltaTime * 30.0f;
    }

    entity.transform.setEulerAngles(rotation);
}

WASDComponent::WASDComponent(Entity &entity, Component::Context &context) : Component(entity, context) {}

