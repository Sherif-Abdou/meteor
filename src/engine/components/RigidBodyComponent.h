//
// Created by sherif on 7/22/2023.
//

#ifndef METEOR_RIGIDBODYCOMPONENT_H
#define METEOR_RIGIDBODYCOMPONENT_H


#include "../Component.h"

class RigidBodyComponent: public Component {
public:
    const glm::vec3 &getVelocity() const;

    void setVelocity(const glm::vec3 &velocity);

    const glm::vec3 &getAcceleration() const;

    void setAcceleration(const glm::vec3 &acceleration);

    float getMass() const;

    void setMass(float mass);

    void applyForce(const glm::vec3 &);

    void applyImpulse(const glm::vec3 &);
private:
    constexpr static const char* component_name = "rigidbody_component";
    glm::vec3 velocity;
    glm::vec3 acceleration;
    float mass;
};


#endif //METEOR_RIGIDBODYCOMPONENT_H
