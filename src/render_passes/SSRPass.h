//
// Created by sherif on 7/13/2023.
//

#ifndef METEOR_SSRPASS_H
#define METEOR_SSRPASS_H


#include "RenderPass.h"

class SSRPass: public RenderPass {
private:
    ShaderProgram* shaderProgram;
    ShaderProgram* blurProgram;
    unsigned int fbo;
    unsigned int blurFbo;
public:
    void render() override;

    void init() override;

    ShaderProgram &getShader() override;

    ~SSRPass() override;
};


#endif //METEOR_SSRPASS_H
