//
// Created by sheri on 6/25/2023.
//

#include "ShadowPass.h"

void ShadowPass::init() {
    shader_program = new ShaderProgram("shaders/light.vert", "shaders/light.frag");

    glGenFramebuffers(1, &depth_map_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, depth_map_fbo);
    unsigned int depth_map;
    glGenTextures(1, &depth_map);
    glBindTexture(GL_TEXTURE_2D, depth_map);

    glEnable(GL_DEPTH_TEST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, (GLsizei)width, (GLsizei)height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_map, 0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw std::runtime_error("FBO Error: " + std::to_string(glCheckFramebufferStatus(GL_FRAMEBUFFER)));
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    texture_outputs["shadow_map"] = depth_map;
}

void ShadowPass::render() {
    glBindFramebuffer(GL_FRAMEBUFFER, depth_map_fbo);
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    shader_program->applyProgram();

    glm::mat4 lightProjection;
    glm::mat4 lightModelView;
    generate_matrices(lightProjection, lightModelView);
    shader_program->setPerspectiveMatrix(lightProjection);
    shader_program->setViewMatrix(lightModelView);
    for (auto& [_, object] : object_inputs) {
        object->addObjectUniformsTo(*shader_program);
        shader_program->addUniforms();
        object->raw_render();
    }
    glCullFace(GL_BACK);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_outputs["shadow_map"]);
}

ShaderProgram &ShadowPass::getShader() {
    return *shader_program;
}

ShadowPass::~ShadowPass() {
    delete shader_program;
}

void ShadowPass::generate_matrices(glm::mat4 &lightProjection, glm::mat4 &lightModelView) const {
    lightProjection= glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 20.0f);
    lightModelView= glm::lookAt(light_position, glm::vec3(0.0), glm::vec3(0.0f, 0.0f, 1.0f));
}

glm::mat4 ShadowPass::getLightSpaceMatrix() {
    glm::mat4 lightProjection;
    glm::mat4 lightModelView;
    generate_matrices(lightProjection, lightModelView);
    return lightProjection * lightModelView;
}

void ShadowPass::addUniformsToPipeline(UniformObject & uniforms) {
    uniforms.mat4Uniforms["uLightSpaceMatrix"] = getLightSpaceMatrix();
}

ShadowPass::ShadowPass(glm::vec3 position): light_position(position) {

}
