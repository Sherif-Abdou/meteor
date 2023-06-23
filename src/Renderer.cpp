//
// Created by sheri on 6/22/2023.
//

#include "Renderer.h"

const int WIDTH= 1920;
const int HEIGHT= 1080;

void Renderer::render_to_window() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0,0,WIDTH,HEIGHT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_FRAMEBUFFER_SRGB);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_CULL_FACE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    raw_render(false);
}

void Renderer::raw_render(bool custom_shader) {
    for (auto& object: objects) {
        if (!custom_shader) {
            object.render();
        } else {
            object.raw_render();
        }
    }
}

void Renderer::raw_render(ShaderProgram & custom_shader) {
    for (auto& object: objects) {
        object.addObjectUniformsTo(custom_shader);
        custom_shader.addUniforms();
        object.raw_render();
    }
}
