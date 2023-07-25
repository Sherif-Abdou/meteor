//
// Created by sherif on 7/9/2023.
//

#ifndef METEOR_COREENGINE_H
#define METEOR_COREENGINE_H

#include <utility>
#include <vector>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <thread>
#include "Entity.h"
#include "../RenderPipeline.h"
#include "physics/CollisionManager.h"
#include "physics/RigidBodyManager.h"
#include <unordered_map>

class CoreEngine {
private:
    std::mutex m;
    void physics_loop();
    bool physics_enabled = true;
    Component::Context context {};
    CollisionManager collisionManager {};
    RigidBodyManager rigidBodyManager {};
    std::unordered_map<unsigned int, Entity*> collider_map {};
    CollisionManager::MatchList previousCollisions;

    void handleCollisions(const CollisionManager::MatchList& collisions);
    void handleRigidBodyCollision(std::pair<unsigned int, unsigned int>);
public:
    explicit CoreEngine();
    explicit CoreEngine(RenderPipeline pipeline);
    explicit CoreEngine(RenderPipeline pipeline, RenderPipeline transparency);

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
    RenderPipeline transparent_pipeline {};
    float physicsUpdateFrequency = 240.0f;
    std::unique_ptr<std::thread> physics_thread;
    void frame_init();
    void frame_loop();
    void start_physics();
    void end_physics();
    Entity& createEntityFromOBJPath(const std::string&, const std::string&);
    Entity& createTransparentEntityFromOBJPath(const std::string&, const std::string&);

    virtual ~CoreEngine();
};


#endif //METEOR_COREENGINE_H
