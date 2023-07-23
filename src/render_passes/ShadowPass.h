//
// Created by sherif on 6/25/2023.
//

#ifndef METEOR_SHADOWPASS_H
#define METEOR_SHADOWPASS_H


#include "RenderPass.h"

class ShadowPass: public RenderPass {
private:
    ShaderProgram* shader_program = nullptr;
    unsigned int depth_map_fbo = 0;
    void generate_matrices(glm::mat4 &lightProjection, glm::mat4 &lightModelView) const;
    glm::mat4 getLightSpaceMatrix();
public:
    explicit ShadowPass(glm::vec3);
    float width = 1024.0f;
    float height = 1024.0f;
    const glm::vec3 light_position;
    void render() override;

    void init() override;

    ShaderProgram &getShader() override;

    ~ShadowPass() override;

    void addUniformsToPipeline(UniformObject &) override;
};


#endif //METEOR_SHADOWPASS_H
