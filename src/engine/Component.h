//
// Created by sheri on 7/6/2023.
//

#ifndef METEOR_COMPONENT_H
#define METEOR_COMPONENT_H

#include <string>

class Entity;

class Component {
protected:
    Entity& entity;
public:
    constexpr static const char* component_name = "component";
    explicit Component(Entity&);
    virtual void init() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void physics_init() {}
    virtual void physics_update(float deltaTime) {}
    virtual const char* getComponentName() = 0;

    virtual ~Component() = default;
};

#include "Entity.h"
#endif //METEOR_COMPONENT_H
