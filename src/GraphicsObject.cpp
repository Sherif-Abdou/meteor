//
// Created by sheri on 6/21/2023.
//

#include "GraphicsObject.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void GraphicsObject::generateVAO() {
    long len = buffer.size();

    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, len*sizeof(float), buffer.data(), GL_DYNAMIC_DRAW);


    // Vertex
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 11*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Normal
    glVertexAttribPointer(1, 3, GL_FLOAT, false, 11*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    // Tex Coord
    glVertexAttribPointer(2, 2, GL_FLOAT, false, 11*sizeof(float), (void*)(9*sizeof(float)));
    glEnableVertexAttribArray(2);

    // Tangent
    glVertexAttribPointer(3, 3, GL_FLOAT, false, 11*sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(3);
    this->vao = vao;
    this->vbo = vbo;
}

void GraphicsObject::addObjectUniformsTo(ShaderProgram& shader_program, bool applyTextures) {
    auto transformMatrix = glm::identity<glm::mat4>();
    transformMatrix = glm::translate(transformMatrix, translation);

    transformMatrix = glm::rotate(transformMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    transformMatrix = glm::rotate(transformMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    transformMatrix = glm::rotate(transformMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    transformMatrix = glm::scale(transformMatrix, scale);
    auto normalTransform = glm::inverseTranspose(transformMatrix);

    shader_program.addMatrix4Uniform("uTransform", transformMatrix);
    shader_program.addMatrix4Uniform("uNormalTransform", normalTransform);

    if (applyTextures) {
        if (albedoTexturePath.has_value()) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, albedoTexture);
            shader_program.addIntegerUniform("albedo", 0);
        } else {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, GL_NONE);
        }
        if (normalTexturePath.has_value()) {
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, normalTexture);
            shader_program.addIntegerUniform("normalMap", 1);
        } else {
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, GL_NONE);
        }
    }
}

void GraphicsObject::raw_render() {
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, vertices);
}

void GraphicsObject::setAlbedoTexture(std::string path) {
    albedoTexturePath = std::optional<std::string>(path);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    glGenTextures(1, &albedoTexture);
    glBindTexture(GL_TEXTURE_2D, albedoTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
}

void GraphicsObject::setNormalTexture(std::string path) {
    normalTexturePath = std::optional<std::string>(path);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    glGenTextures(1, &normalTexture);
    glBindTexture(GL_TEXTURE_2D, normalTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
}

GraphicsObject::~GraphicsObject() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

GraphicsObject::GraphicsObject(OBJFile & obj) {
    vertices = obj.size();
    buffer = obj.createBuffer();
    generateVAO();
}
