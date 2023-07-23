//
// Created by sherif on 7/12/2023.
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
    glDepthMask(GL_FALSE);
    auto object = object_inputs["skybox_cube"];


    glm::mat4 modelViewMatrix = skybox_shader->getViewMatrix();
    skybox_shader->setViewMatrix(glm::mat4(glm::mat3(modelViewMatrix)));

    skybox_shader->applyProgram();
    object->addObjectUniformsTo(*skybox_shader);
    skybox_shader->addUniforms();
    object->raw_render();

    glDepthMask(GL_TRUE);
}

void SkyboxPass::init() {
    skybox_shader = new ShaderProgram("shaders/skybox.vert", "shaders/skybox.frag");

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
