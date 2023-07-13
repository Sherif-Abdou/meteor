//
// Created by sheri on 7/13/2023.
//

#include "SSRPass.h"

void SSRPass::render() {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_inputs["position"]);
    shaderProgram->addIntegerUniform("position", 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture_inputs["normal"]);
    shaderProgram->addIntegerUniform("normal", 1);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture_inputs["albedo"]);
    shaderProgram->addIntegerUniform("albedo", 2);

    auto object = object_inputs["deferred_quad"];

    shaderProgram->applyProgram();
    object->addObjectUniformsTo(*shaderProgram);
    shaderProgram->addUniforms();
    object->raw_render();
}

void SSRPass::init() {
    shaderProgram = new ShaderProgram("shaders/deferred.vert", "shaders/ssr.frag");

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    texture_outputs["ssr"] = texture;
}

ShaderProgram &SSRPass::getShader() {
    return *shaderProgram;
}

SSRPass::~SSRPass() {
    delete shaderProgram;
}
