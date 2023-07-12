//
// Created by sheri on 6/26/2023.
//

#include <random>
#include "SSAOPass.h"

void SSAOPass::render() {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_inputs["position"]);
    shaderProgram->addIntegerUniform("position", 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture_inputs["normal"]);
    shaderProgram->addIntegerUniform("normal", 1);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, noiseTexture);
    shaderProgram->addIntegerUniform("noiseMap", 2);

    auto slot = glGetUniformLocation(shaderProgram->getShaderProgramId(), "raw_samples");

    glViewport(0,0,width,height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto object = object_inputs["deferred_quad"];

    shaderProgram->applyProgram();
    object->addObjectUniformsTo(*shaderProgram);
    shaderProgram->addUniforms();
    glUniform1fv(slot, ssaoKernelRaw.size(), ssaoKernelRaw.data());
    object->raw_render();

    glBindFramebuffer(GL_FRAMEBUFFER, blurFbo);
    glViewport(0,0,width,height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_outputs["tmp_ssao"]);
    shaderProgram->addIntegerUniform("ssao", 0);

    blurProgram->applyProgram();
    object->addObjectUniformsTo(*blurProgram);
    blurProgram->addUniforms();
    object->raw_render();
}

void SSAOPass::init() {
    shaderProgram = new ShaderProgram("shaders/deferred.vert", "shaders/ssao.frag");
    blurProgram = new ShaderProgram("shaders/deferred.vert", "shaders/blur.frag");

    generateKernelSamples();
    generateKernelRotations();
    glm::mat4 perspectiveMatrix = glm::perspectiveFov(glm::radians(90.0f), width,height, 0.1f, 100.0f);
    shaderProgram->setPerspectiveMatrix(perspectiveMatrix);
    glm::mat4 modelViewMatrix = glm::mat4(1.0f);
    modelViewMatrix = glm::translate(modelViewMatrix, glm::vec3(0.0, 0.0, -5.));
    shaderProgram->setViewMatrix(modelViewMatrix);

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);


    glGenFramebuffers(1, &blurFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, blurFbo);
    glGenTextures(1, &blurTexture);
    glBindTexture(GL_TEXTURE_2D, blurTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, blurTexture, 0);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    texture_outputs["tmp_ssao"] = texture;
    texture_outputs["ssao"] = blurTexture;
}

ShaderProgram &SSAOPass::getShader() {
    return *shaderProgram;
}

void SSAOPass::generateKernelSamples() {
    std::uniform_real_distribution<float> randomFloats(0.0, 1.0); // random floats between [0.0, 1.0]
    std::default_random_engine generator;
    for (unsigned int i = 0; i < 64; ++i)
    {
        glm::vec3 sample(
                randomFloats(generator) * 2.0 - 1.0,
                randomFloats(generator) * 2.0 - 1.0,
                randomFloats(generator)
        );
        sample  = glm::normalize(sample);
        float scale = (float)i / 64.0;
        scale   = lerp(0.1f, 1.0f, scale * scale);
        sample *= scale;
        ssaoKernel.push_back(sample);
        ssaoKernelRaw.push_back(sample.x);
        ssaoKernelRaw.push_back(sample.y);
        ssaoKernelRaw.push_back(sample.z);
    }
}

float SSAOPass::lerp(float a, float b, float f) {
    return a + f * (b - a);
}

void SSAOPass::generateKernelRotations() {
    const int width = 4;
    const int height = 4;
    std::vector<glm::vec3> ssaoNoise;
    std::uniform_real_distribution<float> randomFloats(0.0, 1.0); // random floats between [0.0, 1.0]
    std::default_random_engine generator;
    generator.seed(2);
    for (unsigned int i = 0; i < width * height; i++)
    {
        glm::vec3 noise(
                randomFloats(generator) * 2.0 - 1.0,
                randomFloats(generator) * 2.0 - 1.0,
                0.0f);
        ssaoNoise.push_back(noise);
    }
    glGenTextures(1, &noiseTexture);
    glBindTexture(GL_TEXTURE_2D, noiseTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}
