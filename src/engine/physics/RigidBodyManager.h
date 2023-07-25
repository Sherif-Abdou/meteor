//
// Created by sherif on 7/22/2023.
//

#ifndef METEOR_RIGIDBODYMANAGER_H
#define METEOR_RIGIDBODYMANAGER_H


#include <vector>
#include "../components/RigidBodyComponent.h"

class RigidBodyManager {
public:
    std::vector<RigidBodyComponent*> bodies;
    void handleCollision(RigidBodyComponent &a, RigidBodyComponent &b, glm::vec3 intersection);
private:
    std::pair<float, float> handleCollisionAxis(float m1, float v1, float m2, float v2);

    std::pair<glm::vec3, glm::vec3> collisionMath(RigidBodyComponent &a, RigidBodyComponent &b, glm::vec3 intersection);

};


#endif //METEOR_RIGIDBODYMANAGER_H
