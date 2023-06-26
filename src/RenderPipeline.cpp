//
// Created by sheri on 6/25/2023.
//

#include "RenderPipeline.h"

void RenderPipeline::render() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    for (auto pass : passes) {
        addTexturesToRenderPass(*pass);
        addObjectsToRenderPass(*pass);


        pass->init();
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

