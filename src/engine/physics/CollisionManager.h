//
// Created by sheri on 7/14/2023.
//

#ifndef METEOR_COLLISIONMANAGER_H
#define METEOR_COLLISIONMANAGER_H

#include <vector>
#include <algorithm>
#include <set>
#include "AABB.h"

class CollisionManager {
public:
    using MatchSet = std::set<std::pair<unsigned int, unsigned int>>;
    using MatchList = std::vector<std::pair<unsigned int, unsigned int>>;
private:
    std::vector<AABB*> colliders {};
    MatchSet checkAxisOverlaps(std::vector<AABBItem> axis);
public:
    void addCollider(AABB*);
    MatchList checkForCollisions();
};


#endif //METEOR_COLLISIONMANAGER_H
