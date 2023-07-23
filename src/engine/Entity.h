//
// Created by sherif on 7/6/2023.
//

#ifndef METEOR_ENTITY_H
#define METEOR_ENTITY_H


#include "Transform.h"
#include "Component.h"
#include <vector>
#include <memory>
#include <stdexcept>

class Entity {
private:
    std::vector<std::unique_ptr<Component>> components;
    std::string name;
public:
    const std::string &getName() const;

    void setName(const std::string &name);

public:
    Transform transform {};
    Entity();
    Entity(const Entity&) = delete;
    Entity(Entity&&) = default;
    Entity& operator=(Entity&&) = default;


    void init();
    void update(float deltaTime);
    void physics_init();
    void physics_update(float deltaTime);

    void on_collision(Component::CollisionContext);

    void addComponent(std::unique_ptr<Component>);

    template<typename T>
    T& getComponent() {
        for (auto& component : components) {
            if (component->getComponentName() == T::getClassComponentName()) {
                Component* raw = component.get();
                return *dynamic_cast<T*>(raw);
            }
        }
        throw std::runtime_error("Component doesn't exist");
    }

    template<typename T>
    bool hasComponent() {
        for (auto& component : components) {
            if (component->getComponentName() == T::getClassComponentName()) {
                return true;
            }
        }
        return false;
    }
};


#endif //METEOR_ENTITY_H
