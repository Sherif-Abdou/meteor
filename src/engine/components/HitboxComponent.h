//
// Created by sheri on 7/15/2023.
//

#ifndef METEOR_HITBOXCOMPONENT_H
#define METEOR_HITBOXCOMPONENT_H


#include "../Component.h"
#include "../physics/AABB.h"

class HitboxComponent: public Component {
private:
    AABB hitbox;
    void updateHitbox();
    constexpr static const char* component_name = "hitbox_component";
public:
    float height = 1.0;
    float width = 1.0;
    float depth = 1.0;
    glm::vec3 offset = glm::vec3(0.0f);
    bool isColliding = false;

    HitboxComponent(Entity &entity, Context &context);

    void init() override;

    void update(float deltaTime) override;

    const char *getComponentName() override;
    static const char *getClassComponentName();

    void physics_init() override;

    AABB* getHitbox();
    unsigned int getHitboxId();

    void physics_update(float deltaTime) override;

};


#endif //METEOR_HITBOXCOMPONENT_H
