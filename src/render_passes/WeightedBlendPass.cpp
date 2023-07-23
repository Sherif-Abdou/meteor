//
// Created by sherif on 7/20/2023.
//

#include "WeightedBlendPass.h"

void WeightedBlendPass::render() {
    secondShaderProgram->setPerspectiveMatrix(firstShaderProgram->getPerspectiveMatrix());
    secondShaderProgram->setViewMatrix(firstShaderProgram->getViewMatrix());
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, width, height);
    glClear(GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    glBlendFunci(1, GL_ONE, GL_ONE);
    glBlendFunci(2, GL_ZERO, GL_ONE_MINUS_SRC_COLOR);
    float fzeros[] = {0, 0, 0, 0};
    float fones[] = {1, 1, 1, 1};
    glClearBufferfv(GL_COLOR, GL_COLOR_ATTACHMENT0, fzeros);
    glClearBufferfv(GL_COLOR, GL_COLOR_ATTACHMENT1, fones);

    for (auto& [name, object]: object_inputs) {
        object->addObjectUniformsTo(*firstShaderProgram, false);
        firstShaderProgram->addUniforms();
        object->raw_render();
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0,0,width,height);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, accumTexture);
    secondShaderProgram->addIntegerUniform("accumTexture", 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, revealTexture);
    secondShaderProgram->addIntegerUniform("revealTexture", 0);

    for (auto& [name, object]: object_inputs) {
        object->addObjectUniformsTo(*secondShaderProgram, false);
        secondShaderProgram->addUniforms();
        object->raw_render();
    }
}

void WeightedBlendPass::init() {
    firstShaderProgram = new ShaderProgram("geometry.vert", "weighted_blend.frag");
    secondShaderProgram = new ShaderProgram("geometry.vert", "weighted_blend_final.frag");

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glGenBuffers(1, &accumTexture);
    glBindTexture(GL_TEXTURE_2D, accumTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, accumTexture, 0);

    glGenBuffers(1, &revealTexture);
    glBindTexture(GL_TEXTURE_2D, revealTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, revealTexture, 0);

    unsigned int attachments[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
    glDrawBuffers(2, attachments);
}

ShaderProgram &WeightedBlendPass::getShader() {
    return *firstShaderProgram;
}

WeightedBlendPass::~WeightedBlendPass() {
    delete firstShaderProgram;
    delete secondShaderProgram;
}
