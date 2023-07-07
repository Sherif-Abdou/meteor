//
// Created by sheri on 6/26/2023.
//

#ifndef METEOR_GEOMETRYPASS_H
#define METEOR_GEOMETRYPASS_H


#include "RenderPass.h"

class GeometryPass: public RenderPass {
private:
    unsigned int gFBO = 0;
    ShaderProgram* shaderProgram = nullptr;
public:
    glm::vec3 translation {};
    glm::vec3 rotation {};
    void render() override;

    void init() override;

    ShaderProgram &getShader() override;

    ~GeometryPass() override;
};


#endif //METEOR_GEOMETRYPASS_H
