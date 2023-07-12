//
// Created by sheri on 7/12/2023.
//

#include <iostream>
#include "SkyboxPass.h"
#include "stb_image.h"

void SkyboxPass::render() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glCullFace(GL_FRONT);
    glDepthFunc(GL_LEQUAL);
    glViewport(0, 0, width, height);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox);
    auto object = object_inputs["skybox_cube"];

    skybox_shader->applyProgram();
    object->addObjectUniformsTo(*skybox_shader);
    skybox_shader->addUniforms();
    object->raw_render();
}

void SkyboxPass::init() {
    skybox_shader = new ShaderProgram("shaders/skybox.vert", "shaders/skybox.frag");

    glm::mat4 perspectiveMatrix = glm::perspectiveFov(glm::radians(90.0f), width,height, 0.01f, 10.0f);
    skybox_shader->setPerspectiveMatrix(perspectiveMatrix);
    glm::mat4 modelViewMatrix = glm::mat4(1.0f);
//    modelViewMatrix = glm::translate(modelViewMatrix, glm::vec3(0.0, 0.0, -5.));
    skybox_shader->setViewMatrix(modelViewMatrix);

    glGenTextures(1, &skybox);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox);
    int width, height, nrChannels;
//    stbi_set_flip_vertically_on_load(true);
    unsigned char* data;
    for (unsigned int i = 0; i < 6; i++) {
        data = stbi_load(skybox_path[i].c_str(), &width, &height, &nrChannels,0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        } else {
            std::cerr << "Couldn't load skybox image\n";
            stbi_image_free(data);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}

ShaderProgram &SkyboxPass::getShader() {
    return *skybox_shader;
}

SkyboxPass::~SkyboxPass() {
    delete skybox_shader;
}

void SkyboxPass::setSkyboxPaths(const std::string *paths) {
    for (int i = 0; i < 6; i++) {
        skybox_path[i] = paths[i];
    }
}
