//
// Created by sherif on 6/25/2023.
//

#include "ForwardPass.h"

void ForwardPass::init() {
    constexpr float WIDTH = 1920.0f;
    constexpr float HEIGHT = 1080.0f;

    shader_program = new ShaderProgram("shaders/geometry.vert", "shaders/basic_transparency.frag");
}

void ForwardPass::render() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    if (transparencyMode) {
//        glDepthMask(GL_FALSE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    glViewport(0,0,(GLsizei)width,(GLsizei)height);

    std::map<float, GraphicsObject*> objects {};

    for (auto& [_, object] : object_inputs) {
        float distance = glm::length(pipeline->getCameraTranslation() - object->translation);
        objects[distance] = object;
    }
    shader_program->applyProgram();
    for (auto it = objects.rbegin(); it != objects.rend(); ++it) {
        auto object = it->second;
        object->addObjectUniformsTo(*shader_program);
        shader_program->addUniforms();
        object->raw_render();
    }
    if (transparencyMode) {
//        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
    }
    glDepthFunc(GL_LESS);
}

ShaderProgram &ForwardPass::getShader() {
    return *shader_program;
}

ForwardPass::~ForwardPass() {
    delete shader_program;
}


