//
// Created by sheri on 6/25/2023.
//

#include "RenderPipeline.h"

void RenderPipeline::render() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    for (auto pass : passes) {
        addTexturesToRenderPass(*pass);
        addObjectsToRenderPass(*pass);


        uniforms.addToShader(pass->getShader());
        pass->render();
        pass->addUniformsToPipeline(uniforms);
        textures.insert(pass->getOutputTextures().begin(), pass->getOutputTextures().end());
    }
}

void RenderPipeline::addTexturesToRenderPass(RenderPass & pass) {
    for (auto& [name, slot] : textures) {
        pass.setInputTexture(name, slot);
    }
}

void RenderPipeline::addObjectsToRenderPass(RenderPass & pass) {
    for (auto& [name, object] : graphics_object) {
        pass.setInputObject(name, object);
    }
}

void RenderPipeline::addPass(RenderPass* pass) {
    passes.push_back(pass);
}

void RenderPipeline::init() {
    for (auto pass: passes) {
        pass->init();
    }
}

const glm::vec3 &RenderPipeline::getCameraTranslation() const {
    return camera_translation;
}

void RenderPipeline::setCameraTranslation(const glm::vec3 &cameraTranslation) {
    camera_translation = cameraTranslation;
    uniforms.vec3Uniforms["uEyePosition"] = cameraTranslation;
    calculateMatrices();
}

const glm::vec3 &RenderPipeline::getCameraRotation() const {
    return camera_rotation;
}

void RenderPipeline::setCameraRotation(const glm::vec3 &cameraRotation) {
    camera_rotation = cameraRotation;
    calculateMatrices();
}

void RenderPipeline::calculateMatrices() {
    auto viewMatrix = glm::mat4(1.0f);
    viewMatrix = glm::translate(viewMatrix, getCameraTranslation());
    viewMatrix = glm::rotate(viewMatrix, glm::radians(camera_rotation.x), glm::vec3(1, 0, 0));
    viewMatrix = glm::rotate(viewMatrix, glm::radians(camera_rotation.y), glm::vec3(0, 1, 0));
    viewMatrix = glm::rotate(viewMatrix, glm::radians(camera_rotation.z), glm::vec3(0, 0, 1));
    uniforms.viewMatrix = viewMatrix;
}

