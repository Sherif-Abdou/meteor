//
// Created by sheri on 7/9/2023.
//

#include <iostream>
#include "CoreEngine.h"
#include "components/MeshComponent.h"
#include "components/HitboxComponent.h"

void CoreEngine::frame_init() {
    pipeline.init();
    for (auto& entity: entities) {
        entity->init();
    }
}

void CoreEngine::frame_loop() {
    int frame_counter = 0;
    auto previous_time = glfwGetTime();
    double current_time;
    auto last_second = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        current_time = glfwGetTime();
        for (auto& entity: entities) {
            entity->update(current_time - previous_time);
        }
        pipeline.render();

        previous_time = current_time;
        glfwSwapBuffers(window);
        glfwPollEvents();
        frame_counter++;
        if (glfwGetTime() - last_second > 1.0) {
            std::cout << "FPS: " << frame_counter << "\n";
            last_second = glfwGetTime();
            frame_counter = 0;
        }
    }
}

Entity& CoreEngine::createEntityFromOBJPath(const std::string & name, const std::string & obj_path) {
    auto entity = std::make_unique<Entity>();
    entity->setName(name);
    std::unique_ptr<MeshComponent> mesh = std::make_unique<MeshComponent>(*entity, context, pipeline);
    mesh->setObjPath(obj_path);
    std::unique_ptr<HitboxComponent> hitbox = std::make_unique<HitboxComponent>(*entity, context);
    collisionManager.addCollider(hitbox->getHitbox());
    auto id = hitbox->getHitboxId();

    entity->addComponent(std::move(mesh));
    entity->addComponent(std::move(hitbox));
    entities.push_back(std::move(entity));

    collider_map[id] = entities[entities.size() - 1].get();
    return *entities[entities.size() - 1];
}

void CoreEngine::start_physics() {
    for (auto& entity: entities) {
        entity->physics_init();
    }
    physics_enabled = true;
    physics_thread = std::make_unique<std::thread>(&CoreEngine::physics_loop, this);
}

void CoreEngine::physics_loop() {
    auto previous_time = glfwGetTime();
    auto current_time = glfwGetTime();
    auto previous_second_time = glfwGetTime();

    unsigned int ticks = 0;

    while (this->physics_enabled) {
        auto collisions = collisionManager.checkForCollisions();
        if (!collisions.empty()) {
            handleCollisions(std::move(collisions));
        }
        current_time = glfwGetTime();
        if (current_time - previous_time > 1/physicsUpdateFrequency) {
            for (auto& entity: entities) {
                entity->physics_update(current_time - previous_time);
            }
            ticks++;

            previous_time = current_time;
        }
        if (current_time - previous_second_time > 1.0) {
            std::cout << "TPS: " << ticks << "\n";
            ticks = 0;
            previous_second_time = current_time;
        }
    }
}

void CoreEngine::end_physics() {
    physics_enabled = false;
    physics_thread->join();
}

GLFWwindow *CoreEngine::getWindow() const {
    return window;
}

void CoreEngine::setWindow(GLFWwindow *window) {
    CoreEngine::window = window;
    CoreEngine::context.window = window;
}

Component::Context &CoreEngine::getContext() {
    return context;
}

CoreEngine::CoreEngine() {
    camera = new Camera(pipeline);
    context.camera = camera;
}

CoreEngine::~CoreEngine() {
    context.camera = nullptr;
    delete camera;
}

Camera *CoreEngine::getCamera() const {
    return camera;
}

void CoreEngine::handleCollisions(CollisionManager::MatchList collisions) {
    for (auto& [_, entity]: collider_map) {
        entity->getComponent<HitboxComponent>().isColliding = false;
    }
    for (auto& [first_id, second_id]: collisions) {
        collider_map[first_id]->getComponent<HitboxComponent>().isColliding = true;
        collider_map[second_id]->getComponent<HitboxComponent>().isColliding = true;
    }
}
