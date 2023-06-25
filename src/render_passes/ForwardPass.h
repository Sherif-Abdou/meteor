//
// Created by sheri on 6/25/2023.
//

#ifndef METEOR_FORWARDPASS_H
#define METEOR_FORWARDPASS_H


#include "RenderPass.h"

class ForwardPass: public RenderPass {
public:
    ForwardPass() = default;

    void init() override;
    void render() override;
    ShaderProgram &getShader() override;

    ~ForwardPass() override;
private:
    ShaderProgram* shader_program{};
};


#endif //METEOR_FORWARDPASS_H