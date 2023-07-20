//
// Created by sheri on 6/21/2023.
//

#ifndef METEOR_GRAPHICSOBJECT_H
#define METEOR_GRAPHICSOBJECT_H

#include <optional>
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
    unsigned int vbo;
    unsigned int vertices;
    unsigned int albedoTexture;
    unsigned int normalTexture;

    void generateVAO();
    std::optional<std::string> albedoTexturePath {};
    std::optional<std::string> normalTexturePath {};
    glm::vec4 solidAlbedo = glm::vec4(0.0, 0.0, 0.0, 1.0);
public:
    const glm::vec4 &getSolidAlbedo() const;

    void setSolidAlbedo(const glm::vec4 &solidAlbedo);

public:
    void setAlbedoTexture(std::string path);
    void setNormalTexture(std::string path);
    GraphicsObject(const GraphicsObject&) = delete;
    GraphicsObject(GraphicsObject&&) = default;
    GraphicsObject& operator=(GraphicsObject&&) = default;
    bool isTransparent = false;
    glm::vec3 translation {};
    glm::vec3 rotation {};
    glm::vec3 scale = glm::vec3(1.0f);
    explicit GraphicsObject(OBJFile&);
    void raw_render();
    void addObjectUniformsTo(ShaderProgram&, bool applyTextures=false);

    ~GraphicsObject();
};


#endif //METEOR_GRAPHICSOBJECT_H
