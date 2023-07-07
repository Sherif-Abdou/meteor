//
// Created by sheri on 6/25/2023.
//

#include "ForwardPass.h"

void ForwardPass::init() {
    constexpr float WIDTH = 1920.0f;
    constexpr float HEIGHT = 1080.0f;

    shader_program = new ShaderProgram("shaders/vertex.vert", "shaders/fragment.frag");

    glm::mat4 modelViewMatrix = glm::mat4(1.0f);
    modelViewMatrix = glm::translate(modelViewMatrix, glm::vec3(0.0, 0.0, -5.));
    glm::mat4 perspectiveMatrix = glm::perspectiveFov(glm::radians(90.0f), WIDTH,HEIGHT, 0.1f, 100.0f);

    shader_program->setPerspectiveMatrix(perspectiveMatrix);
    shader_program->setViewMatrix(modelViewMatrix);
}

void ForwardPass::render() {
    glViewport(0,0,(GLsizei)width,(GLsizei)height);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindTexture(GL_TEXTURE0, texture_inputs["shadow_map"]);
    shader_program->applyProgram();

    for (auto& [_, object] : object_inputs) {
        object->addObjectUniformsTo(*shader_program);
        shader_program->addUniforms();
        object->raw_render();
    }
}

ShaderProgram &ForwardPass::getShader() {
    return *shader_program;
}

ForwardPass::~ForwardPass() {
    delete shader_program;
}


