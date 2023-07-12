//
// Created by sheri on 6/26/2023.
//

#include "GeometryPass.h"

void GeometryPass::render() {
    if (shaderProgram == nullptr) {
        throw std::runtime_error("Shader program not set");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, gFBO);
    glViewport(0,0,(GLsizei)width, (GLsizei)height);

    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shaderProgram->applyProgram();

    glm::mat4 modelViewMatrix = glm::mat4(1.0f);
    modelViewMatrix = glm::translate(modelViewMatrix, -translation);

    modelViewMatrix = glm::rotate(modelViewMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    modelViewMatrix = glm::rotate(modelViewMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    modelViewMatrix = glm::rotate(modelViewMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    glm::mat4 perspectiveMatrix = glm::perspectiveFov(glm::radians(90.0f), width,height, 0.1f, 100.0f);

    shaderProgram->setPerspectiveMatrix(perspectiveMatrix);
    shaderProgram->setViewMatrix(modelViewMatrix);

    for (auto& [name, object] : object_inputs) {
        if (name == "deferred_quad" || name == "skybox_cube") {
            continue;
        }
        object->addObjectUniformsTo(*shaderProgram, true);
        shaderProgram->addUniforms();
        object->raw_render();
    }
}

void GeometryPass::init() {
    shaderProgram = new ShaderProgram("shaders/geometry.vert", "shaders/geometry.frag");

    unsigned int positionTexture, normalTexture, albedoTexture, tangentTexture;
    glGenFramebuffers(1, &gFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, gFBO);

    glGenTextures(1, &positionTexture);
    glBindTexture(GL_TEXTURE_2D, positionTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, (GLsizei)width, (GLsizei)height, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, positionTexture, 0);

    glGenTextures(1, &normalTexture);
    glBindTexture(GL_TEXTURE_2D, normalTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, (GLsizei)width, (GLsizei)height, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normalTexture, 0);

    glGenTextures(1, &albedoTexture);
    glBindTexture(GL_TEXTURE_2D, albedoTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, (GLsizei)width, (GLsizei)height, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, albedoTexture, 0);

    glGenTextures(1, &tangentTexture);
    glBindTexture(GL_TEXTURE_2D, tangentTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, (GLsizei)width, (GLsizei)height, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, tangentTexture, 0);

    unsigned int attachments[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
    glDrawBuffers(4, attachments);

    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, (GLsizei)width, (GLsizei)height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    texture_outputs["position"] = positionTexture;
    texture_outputs["normal"] = normalTexture;
    texture_outputs["albedo"] = albedoTexture;
    texture_outputs["tangent"] = tangentTexture;
}

ShaderProgram &GeometryPass::getShader() {
    return *shaderProgram;
}

GeometryPass::~GeometryPass() {
    delete shaderProgram;
}
