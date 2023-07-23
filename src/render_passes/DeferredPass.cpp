//
// Created by sherif on 6/26/2023.
//

#include <iostream>
#include "DeferredPass.h"
#include "stb_image.h"

void DeferredPass::render() {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0,0, (GLsizei) width, (GLsizei) height);
    glDisable(GL_BLEND);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_inputs["position"]);
    shaderProgram->addIntegerUniform("position", 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture_inputs["normal"]);
    shaderProgram->addIntegerUniform("normal", 1);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture_inputs["albedo"]);
    shaderProgram->addIntegerUniform("albedo", 2);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, texture_inputs["shadow_map"]);
    shaderProgram->addIntegerUniform("shadowMap", 3);

    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, texture_inputs["tangent"]);
    shaderProgram->addIntegerUniform("tangent", 4);

    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, texture_inputs["ssao"]);
    shaderProgram->addIntegerUniform("occlusionMap", 5);

    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D, texture_inputs["ssr"]);
    shaderProgram->addIntegerUniform("reflectionMap", 6);


    auto object = object_inputs["deferred_quad"];

    shaderProgram->applyProgram();
    object->addObjectUniformsTo(*shaderProgram);
    shaderProgram->addUniforms();
    object->raw_render();
}

void DeferredPass::init() {
    shaderProgram = new ShaderProgram("shaders/deferred.vert", "shaders/deferred.frag");
    if (!renderToWindow) {
        glGenFramebuffers(1, &fbo);

        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, (GLsizei)width, (GLsizei)height, 0, GL_RGB, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        texture_outputs["deferred_image"] = texture;
    }
}

ShaderProgram &DeferredPass::getShader() {
    return *shaderProgram;
}
