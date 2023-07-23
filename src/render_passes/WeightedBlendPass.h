//
// Created by sherif on 7/20/2023.
//

#ifndef METEOR_WEIGHTEDBLENDPASS_H
#define METEOR_WEIGHTEDBLENDPASS_H


#include "RenderPass.h"

class WeightedBlendPass: public RenderPass {
private:
    ShaderProgram* firstShaderProgram;
    ShaderProgram* secondShaderProgram;
    unsigned int fbo;
    unsigned int accumTexture;
    unsigned int revealTexture;
public:
    void render() override;

    void init() override;

    ShaderProgram &getShader() override;

    ~WeightedBlendPass() override;
};


#endif //METEOR_WEIGHTEDBLENDPASS_H
