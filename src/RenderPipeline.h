//
// Created by sherif on 6/25/2023.
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
    glm::vec3 camera_translation;

    void calculateMatrices();
public:
    const glm::vec3 &getCameraTranslation() const;

    void setCameraTranslation(const glm::vec3 &cameraTranslation);

    const glm::vec3 &getCameraRotation() const;

    void setCameraRotation(const glm::vec3 &cameraRotation);

private:
    glm::vec3 camera_rotation;
public:
    std::map<std::string, GraphicsObject*> graphics_object {};
    std::map<std::string, unsigned int> textures {};
    UniformObject uniforms {};

    void init();
    void render();
    void addPass(RenderPass* pass);
};


#endif //METEOR_RENDERPIPELINE_H
