//
// Created by sheri on 7/13/2023.
//

#include "SSRPass.h"

void SSRPass::render() {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0,0,width,height);
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

    glBindFramebuffer(GL_FRAMEBUFFER, blurFbo);
    glViewport(0,0,width,height);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_inputs["tmp_ssr"]);
    blurProgram->addIntegerUniform("tmp_ssr", 0);

    blurProgram->applyProgram();
    object->addObjectUniformsTo(*blurProgram);
    blurProgram->addUniforms();
    object->raw_render();
}

void SSRPass::init() {
    shaderProgram = new ShaderProgram("shaders/deferred.vert", "shaders/ssr.frag");
    blurProgram = new ShaderProgram("shaders/deferred.vert", "shaders/ssrBlur.frag");

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

    glGenFramebuffers(1, &blurFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, blurFbo);

    unsigned int blurTexture;
    glGenTextures(1, &blurTexture);
    glBindTexture(GL_TEXTURE_2D, blurTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, blurTexture, 0);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    texture_outputs["tmp_ssr"] = texture;
    texture_outputs["ssr"] = blurTexture;
}

ShaderProgram &SSRPass::getShader() {
    return *shaderProgram;
}

SSRPass::~SSRPass() {
    delete shaderProgram;
}
