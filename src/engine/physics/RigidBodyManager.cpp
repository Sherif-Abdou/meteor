//
// Created by sherif on 7/22/2023.
//

#include "RigidBodyManager.h"

void RigidBodyManager::handleCollision(RigidBodyComponent& a, RigidBodyComponent& b, glm::vec3 intersection) {
    auto a_velocity = a.getVelocity();
    auto b_velocity = a.getVelocity();
    auto a_mass = a.getMass();
    auto b_mass = b.getMass();

    if (a_mass + b_mass == 0) {
        return;
    }

//    auto [a_x, b_x] = handleCollisionAxis(a_mass, a_velocity.x, b_mass, b_velocity.x);
//    auto [a_y, b_y] = handleCollisionAxis(a_mass, a_velocity.y, b_mass, b_velocity.y);
//    auto [a_z, b_z] = handleCollisionAxis(a_mass, a_velocity.z, b_mass, b_velocity.z);

    auto [finalA, finalB] = collisionMath(a, b, intersection);

//    a.setVelocity(glm::vec3(a_x, a_y, a_z));
//    b.setVelocity(glm::vec3(b_x, b_y, b_z));
    a.setVelocity(finalA);
    b.setVelocity(finalB);
}

std::pair<float, float> RigidBodyManager::handleCollisionAxis(float m1, float v1, float m2, float v2) {
    float invSum  = 1 / (m1 + m2);
    float diffSum = (m1 - m2) * invSum;

    float v1f = diffSum * v1 + 2 * m2 * invSum * v2;
    float v2f = 2 * m1 * invSum * v1 - diffSum * v2;

    return {v1f, v2f};
}

std::pair<glm::vec3, glm::vec3> RigidBodyManager::collisionMath(RigidBodyComponent &a, RigidBodyComponent &b, glm::vec3 intersection) {
    auto a_velocity = a.getVelocity();
    auto a_position = a.getPosition();
    auto a_mass = a.getMass();
    auto b_velocity = b.getVelocity();
    auto b_position = b.getPosition();
    auto b_mass = b.getMass();

    auto ab_delta = glm::normalize(a_position - b_position);
    auto ba_delta = glm::normalize(b_position - a_position);

    glm::vec3 finalAVelocity = a_velocity - glm::vec3((2 * b_mass) / (a_mass + b_mass))
            * glm::vec3(glm::dot(ab_delta, a_velocity - b_velocity))
            * ab_delta;
    glm::vec3 finalBVelocity = b_velocity - glm::vec3((2 * a_mass) / (a_mass + b_mass))
                                            * glm::vec3(glm::dot(ba_delta, b_velocity - a_velocity))
                                            * ba_delta;

    return {finalAVelocity, finalBVelocity};
}


