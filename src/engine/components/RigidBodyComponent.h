//
// Created by sherif on 7/22/2023.
//

#ifndef METEOR_RIGIDBODYCOMPONENT_H
#define METEOR_RIGIDBODYCOMPONENT_H


#include "../Component.h"

class RigidBodyComponent: public Component {
public:
    RigidBodyComponent(Entity &entity, Context &context);

    const glm::vec3 &getVelocity() const;

    void setVelocity(const glm::vec3 &velocity);

    const glm::vec3 &getAcceleration() const;

    void setAcceleration(const glm::vec3 &acceleration);

    float getMass() const;

    void setMass(float mass);

    void applyForce(const glm::vec3 &);

    void applyImpulse(const glm::vec3 &);

    const char *getComponentName() override;

    static const char *getClassComponentName();

    void init() override;

    void update(float deltaTime) override;

    void physics_update(float deltaTime) override;

    glm::vec3 getPosition();
private:
    constexpr static const char* component_name = "rigidbody_component";
    glm::vec3 velocity {};
    glm::vec3 acceleration {};
    float mass = 1.0f;
};


#endif //METEOR_RIGIDBODYCOMPONENT_H
