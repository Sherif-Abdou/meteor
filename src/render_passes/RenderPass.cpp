//
// Created by sheri on 6/25/2023.
//

#include "RenderPass.h"

void RenderPass::setInputTexture(const std::string & name, unsigned int slot) {
    texture_inputs[name] = slot;
}

void RenderPass::setOutputTexture(const std::string & name, unsigned int slot) {
    texture_outputs[name] = slot;
}

void RenderPass::setInputObject(const std::string & name, GraphicsObject * object) {
    object_inputs[name] = object;
}

