//
// Created by sheri on 6/21/2023.
//

#ifndef METEOR_GRAPHICSOBJECT_H
#define METEOR_GRAPHICSOBJECT_H


#include <vector>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "OBJFile.h"
#include "ShaderProgram.h"
#include <glad/gl.h>


class GraphicsObject {
private:
    std::vector<float> buffer;
    unsigned int vao;
    unsigned int vertices;

    void generateVAO();
    void addObjectUniforms();
    void generateTangents();
public:
    GraphicsObject(const GraphicsObject&) = delete;
    GraphicsObject(GraphicsObject&&) = default;
    GraphicsObject& operator=(GraphicsObject&&) = default;
    glm::vec3 translation {};
    glm::vec3 rotation {};
    glm::vec3 scale = glm::vec3(1.0f);
    ShaderProgram& shader_program;
    explicit GraphicsObject(OBJFile&, ShaderProgram&);
    void render();
    void raw_render();
    void addObjectUniformsTo(ShaderProgram&);
};


#endif //METEOR_GRAPHICSOBJECT_H
