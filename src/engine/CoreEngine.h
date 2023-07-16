//
// Created by sheri on 7/9/2023.
//

#ifndef METEOR_COREENGINE_H
#define METEOR_COREENGINE_H

#include <vector>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <thread>
#include "Entity.h"
#include "../RenderPipeline.h"
#include "physics/CollisionManager.h"
#include <unordered_map>

class CoreEngine {
private:
    void physics_loop();
    bool physics_enabled = true;
    Component::Context context {};
    CollisionManager collisionManager {};
    std::unordered_map<unsigned int, Entity*> collider_map {};

    void handleCollisions(const CollisionManager::MatchList& collisions);
public:
    explicit CoreEngine();

    [[nodiscard]] Component::Context &getContext();

private:
    GLFWwindow* window = nullptr;
    Camera* camera;
public:
    Camera *getCamera() const;

public:
    [[nodiscard]] GLFWwindow *getWindow() const;

    void setWindow(GLFWwindow *window);

public:
    std::vector<std::unique_ptr<Entity>> entities {};
    RenderPipeline pipeline {};
    float physicsUpdateFrequency = 240.0f;
    std::unique_ptr<std::thread> physics_thread;
    void frame_init();
    void frame_loop();
    void start_physics();
    void end_physics();
    Entity& createEntityFromOBJPath(const std::string&, const std::string&);

    virtual ~CoreEngine();
};


#endif //METEOR_COREENGINE_H
