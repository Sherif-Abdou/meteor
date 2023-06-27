//
// Created by sheri on 6/26/2023.
//

#ifndef METEOR_DEFERREDPASS_H
#define METEOR_DEFERREDPASS_H


#include "RenderPass.h"

class DeferredPass: public RenderPass {
private:
    ShaderProgram* shaderProgram;
    unsigned int fbo = 0;
public:
    bool renderToWindow = true;
    void render() override;

    void init() override;

    ShaderProgram &getShader() override;

};


#endif //METEOR_DEFERREDPASS_H
