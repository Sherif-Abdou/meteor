//
// Created by sheri on 7/6/2023.
//

#include "Entity.h"

void Entity::init() {
    for (auto& component: components) {
        component->init();
    }
}

void Entity::update(float deltaTime) {
    for (auto& component: components) {
        component->update(deltaTime);
    }
}

void Entity::physics_init() {
    for (auto& component: components) {
        component->physics_init();
    }
}

void Entity::physics_update(float deltaTime) {
    for (auto& component: components) {
        component->physics_update(deltaTime);
    }
}

void Entity::addComponent(std::unique_ptr<Component> component) {
    this->components.push_back(std::move(component));
}

const std::string &Entity::getName() const {
    return name;
}

void Entity::setName(const std::string &name) {
    this->name = name;
}

Entity::Entity() {

}

void Entity::on_collision(Component::CollisionContext context) {
    for (auto& component : components) {
        component->on_collision(context);
    }
}

//template<typename T>
//T &Entity::getComponent() {
////    for (auto& component : components) {
////        if (component->component_name == T::component_name) {
////            return *component;
//}
//}
//}

