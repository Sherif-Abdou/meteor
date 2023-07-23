//
// Created by sherif on 7/10/2023.
//

#ifndef METEOR_WASDCOMPONENT_H
#define METEOR_WASDCOMPONENT_H


#include "../Component.h"

class WASDComponent: public Component {
private:
    float velocity = 1.0f;
    float gravity = 0.9f;
    double lastCollisionTime;
public:
    float sensitivity = -0.7f;
    WASDComponent(Entity &entity, Context &context);

    void init() override;

    void update(float deltaTime) override;

    const char *getComponentName() override;

    void on_collision(CollisionContext context) override;

    void physics_update(float deltaTime) override;

    void updateCamera(float deltaTime);
};


#endif //METEOR_WASDCOMPONENT_H
