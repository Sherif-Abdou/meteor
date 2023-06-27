//
// Created by sheri on 6/21/2023.
//

#include "GraphicsObject.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GraphicsObject::GraphicsObject(OBJFile& obj, ShaderProgram& shader_program):
    shader_program{shader_program} {
    vertices = obj.size();
    buffer = obj.createBuffer();
    generateVAO();
}

void GraphicsObject::generateVAO() {
    long len = buffer.size();

    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, len*sizeof(float), buffer.data(), GL_STATIC_DRAW);


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
}

void GraphicsObject::render() {
    shader_program.applyProgram();
    addObjectUniforms();
    shader_program.addUniforms();

    raw_render();
}

void GraphicsObject::addObjectUniformsTo(ShaderProgram& shader_program) {
    auto transformMatrix = glm::identity<glm::mat4>();
    transformMatrix = glm::translate(transformMatrix, translation);

    transformMatrix = glm::rotate(transformMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    transformMatrix = glm::rotate(transformMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    transformMatrix = glm::rotate(transformMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    transformMatrix = glm::scale(transformMatrix, scale);
    auto normalTransform = glm::inverseTranspose(transformMatrix);

    shader_program.addMatrix4Uniform("uTransform", transformMatrix);
    shader_program.addMatrix4Uniform("uNormalTransform", normalTransform);
}

void GraphicsObject::addObjectUniforms() {
    addObjectUniformsTo(shader_program);
}

void GraphicsObject::raw_render() {
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, vertices);
}

