//
// Created by sherif on 7/14/2023.
//

#include "AABB.h"

AABB::AABB()  {
    static unsigned int current_id = 0;
    current_id++;
    this->id = current_id;
}

AABBItem AABB::startXItem() {
    return AABBItem {true, startX, id};
}

AABBItem AABB::endXItem() {
    return AABBItem {false, endX, id};
}

AABBItem AABB::startYItem() {
    return AABBItem {true, startY, id};
}

AABBItem AABB::endYItem() {
    return AABBItem {false, endY, id};
}

AABBItem AABB::startZItem() {
    return AABBItem {true, startZ, id};
}

AABBItem AABB::endZItem() {
    return AABBItem {false, endZ, id};
}

unsigned int AABB::getId() const {
    return id;
}

glm::vec3 AABB::center() {
    return {(startX + endX) / 2.0f, (startY + endY) / 2.0f, (startZ + endZ) / 2.0f};
}
