//
// Created by sherif on 7/22/2023.
//

#include "RigidBodyManager.h"

void RigidBodyManager::handleCollision(RigidBodyComponent *a, RigidBodyComponent *b) {
    if (a == nullptr || b == nullptr) {
        return;
    }

    auto a_velocity = a->getVelocity();
    auto b_velocity = a->getVelocity();
    auto a_mass = a->getMass();
    auto b_mass = b->getMass();

    if (a_mass + b_mass == 0) {
        return;
    }

    auto [a_x, b_x] = handleCollisionAxis(a_mass, a_velocity.x, b_mass, b_velocity.x);
    auto [a_y, b_y] = handleCollisionAxis(a_mass, a_velocity.y, b_mass, b_velocity.y);
    auto [a_z, b_z] = handleCollisionAxis(a_mass, a_velocity.z, b_mass, b_velocity.z);

    a->setVelocity(glm::vec3(a_x, a_y, a_z));
    b->setVelocity(glm::vec3(b_x, b_y, b_z));
}

std::pair<float, float> RigidBodyManager::handleCollisionAxis(float m1, float v1, float m2, float v2) {
    float invSum  = 1 / (m1 + m2);
    float diffSum = (m1 - m2) * invSum;

    float v1f = diffSum * v1 + 2 * m2 * invSum * v2;
    float v2f = 2 * m1 * invSum * v1 - invSum * v2;

    return {v1f, v2f};
}
