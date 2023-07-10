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

class CoreEngine {
private:
    void physics_loop();
    bool physics_enabled = true;
public:
    std::vector<std::unique_ptr<Entity>> entities {};
    RenderPipeline pipeline {};
    float physicsUpdateFrequency = 240.0f;
    GLFWwindow* window;
    std::unique_ptr<std::thread> physics_thread;
    void frame_init();
    void frame_loop();
    void start_physics();
    void end_physics();
    Entity& createEntityFromOBJPath(const std::string&, const std::string&);
};


#endif //METEOR_COREENGINE_H
