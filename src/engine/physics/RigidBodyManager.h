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
    void runRigidBodyCollisions();
private:
    void handleCollision(RigidBodyComponent* a, RigidBodyComponent* b);
    std::pair<float, float> handleCollisionAxis(float m1, float v1, float m2, float v2);
};


#endif //METEOR_RIGIDBODYMANAGER_H
