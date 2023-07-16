//
// Created by sheri on 7/10/2023.
//

#ifndef METEOR_WASDCOMPONENT_H
#define METEOR_WASDCOMPONENT_H


#include "../Component.h"

class WASDComponent: public Component {
private:
    float multipler = 1.0f;
public:
    float sensitivity = -0.7f;
    WASDComponent(Entity &entity, Context &context);

    void init() override;

    void update(float deltaTime) override;

    const char *getComponentName() override;

    void physics_update(float deltaTime) override;
};


#endif //METEOR_WASDCOMPONENT_H
