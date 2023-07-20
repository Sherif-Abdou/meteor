//
// Created by sheri on 7/14/2023.
//

#include <iterator>
#include "CollisionManager.h"

CollisionManager::MatchList CollisionManager::checkForCollisions() {
    std::vector<AABBItem> xItems {};
    std::vector<AABBItem> yItems {};
    std::vector<AABBItem> zItems {};

    for (auto collider: colliders) {
        xItems.push_back(collider->startXItem());
        xItems.push_back(collider->endXItem());
        yItems.push_back(collider->startYItem());
        yItems.push_back(collider->endYItem());
        zItems.push_back(collider->startZItem());
        zItems.push_back(collider->endZItem());
    }

    auto xIntersections = checkAxisOverlaps(std::move(xItems));
    auto yIntersections = checkAxisOverlaps(std::move(yItems));
    auto zIntersections = checkAxisOverlaps(std::move(zItems));

    MatchList tmpIntersections {};
    std::set_intersection(xIntersections.begin(), xIntersections.end(),
                   yIntersections.begin(), yIntersections.end(),
                   std::back_inserter(tmpIntersections));
    MatchList finalIntersections {};
    std::set_intersection(tmpIntersections.begin(), tmpIntersections.end(),
                   zIntersections.begin(), zIntersections.end(),
                   std::back_inserter(finalIntersections));

    return finalIntersections;
};

CollisionManager::MatchSet CollisionManager::checkAxisOverlaps(std::vector<AABBItem> axis) {
    std::sort(axis.begin(), axis.end(), [](AABBItem& a, AABBItem& b) {
        return a.value < b.value;
    });

    int i = 0;
    std::set<std::pair<unsigned int, unsigned int>> intersections;
    while (i < axis.size() - 1) {
        auto& current = axis[i];
        if (!current.isStart) {
            i++;
            continue;
        }

        int j = i + 1;
        while (j < axis.size()) {
            auto ahead = axis[j];
            if (ahead.id == current.id && !ahead.isStart) {
                break;
            }
            // Keep intersections formatted consistently, smaller id first
            auto first = std::min(current.id, ahead.id);
            auto last = std::max(current.id, ahead.id);
            intersections.insert({first, last});
            j++;
        }
        i++;
    }

    return intersections;
}

void CollisionManager::addCollider(AABB * collider) {
    this->colliders.push_back(collider);
}
