//
// Created by sheri on 6/22/2023.
//

#ifndef METEOR_SHADOWMAP_H
#define METEOR_SHADOWMAP_H


#include "ShaderProgram.h"
#include "Renderer.h"

class ShadowMap {
private:
    unsigned int width = 1024;
    unsigned int height = 1024;
    unsigned int depth_map;
    unsigned int depth_map_fbo;
    unsigned int depth_map_rbo;
    void applyUniforms();
public:
    ShaderProgram& light_shader;
    glm::vec3 light_position;
    ShadowMap(ShaderProgram&);
    void render_depth_map(Renderer&);
    glm::mat4 getLightSpaceMatrix();

    void generate_matrices(glm::mat4 &lightProjection, glm::mat4 &lightModelView) const;
};


#endif //METEOR_SHADOWMAP_H
