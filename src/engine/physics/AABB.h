//
// Created by sherif on 7/14/2023.
//

#ifndef METEOR_AABB_H
#define METEOR_AABB_H

struct AABBItem {
    bool isStart;
    float value;
    unsigned int id;
};

class AABB {
public:
    explicit AABB();
    AABBItem startXItem();
    AABBItem endXItem();
    AABBItem startYItem();
    AABBItem endYItem();
    AABBItem startZItem();
    AABBItem endZItem();
private:
    unsigned int id;
public:
    unsigned int getId() const;

public:
    float startX;
    float endX;
    float startY;
    float endY;
    float startZ;
    float endZ;
};


#endif //METEOR_AABB_H
