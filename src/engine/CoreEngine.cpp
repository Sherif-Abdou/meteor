//
// Created by sheri on 7/9/2023.
//

#include <iostream>
#include "CoreEngine.h"
#include "components/MeshComponent.h"

void CoreEngine::frame_init() {
    for (auto& entity: entities) {
        entity->init();
    }
    pipeline.init();
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
    std::unique_ptr<MeshComponent> mesh = std::make_unique<MeshComponent>(*entity, pipeline);
    mesh->setObjPath(obj_path);

    entity->addComponent(std::move(mesh));
    entities.push_back(std::move(entity));
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

    while (this->physics_enabled) {
        current_time = glfwGetTime();
        if (current_time - previous_time > 1/physicsUpdateFrequency) {
            for (auto& entity: entities) {
                entity->physics_update(current_time - previous_time);
            }

            previous_time = current_time;
        }
    }
}

void CoreEngine::end_physics() {
    physics_enabled = false;
    physics_thread->join();
}
