//
// Created by sherif on 6/26/2023.
//

#ifndef METEOR_SSAOPASS_H
#define METEOR_SSAOPASS_H


#include "RenderPass.h"

class SSAOPass: public RenderPass {
private:
    ShaderProgram* shaderProgram;
    ShaderProgram* blurProgram;
    unsigned int fbo = 0;
    void generateKernelSamples();
    void generateKernelRotations();
    unsigned int noiseTexture;
    unsigned int blurFbo = 0;
    unsigned int blurTexture;
public:
    void render() override;

    void init() override;

    ShaderProgram &getShader() override;

    float lerp(float a, float b, float f);

    std::vector<glm::vec3> ssaoKernel;
    std::vector<float> ssaoKernelRaw;
};


#endif //METEOR_SSAOPASS_H
