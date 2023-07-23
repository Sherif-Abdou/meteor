//
// Created by sherif on 7/12/2023.
//

#ifndef METEOR_SKYBOXPASS_H
#define METEOR_SKYBOXPASS_H


#include "RenderPass.h"

class SkyboxPass: public RenderPass {
private:
    ShaderProgram* skybox_shader;
    unsigned int skybox;
public:
    std::string skybox_path[6];
    void render() override;

    void init() override;

    ShaderProgram &getShader() override;

    ~SkyboxPass() override;

    void setSkyboxPaths(const std::string *paths);
};


#endif //METEOR_SKYBOXPASS_H
