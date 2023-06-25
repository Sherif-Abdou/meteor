//
// Created by sheri on 6/25/2023.
//

#ifndef METEOR_RENDERPIPELINE_H
#define METEOR_RENDERPIPELINE_H

#include <map>
#include <string>
#include "ShaderProgram.h"
#include "GraphicsObject.h"
#include "render_passes/RenderPass.h"

class RenderPipeline {
private:
    std::vector<RenderPass*> passes;

    void addTexturesToRenderPass(RenderPass&);
    void addObjectsToRenderPass(RenderPass&);
public:
    std::map<std::string, GraphicsObject*> graphics_object;
    std::map<std::string, unsigned int> textures;

    void render();
    void addPass(RenderPass* pass);
};


#endif //METEOR_RENDERPIPELINE_H
