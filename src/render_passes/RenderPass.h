//
// Created by sheri on 6/25/2023.
//

#ifndef METEOR_RENDERPASS_H
#define METEOR_RENDERPASS_H

#include <map>
#include <vector>
#include <string>
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "../GraphicsObject.h"
#include "UniformObject.h"

class RenderPass {
protected:
    std::map<std::string, unsigned int> texture_inputs {};
    std::map<std::string, unsigned int> texture_outputs {};
    std::map<std::string, GraphicsObject*> object_inputs {};
public:
    /// Draw
    virtual void render() = 0;
    /// Generate frame_buffers, textures and other buffers you may need
    virtual void init() = 0;
    virtual void setInputTexture(const std::string&, unsigned int);
    virtual std::map<std::string, unsigned int>& getOutputTextures();
    virtual void setInputObject(const std::string&, GraphicsObject*);
    virtual void addUniformsToPipeline(UniformObject&);
    virtual ShaderProgram& getShader() = 0;

    virtual ~RenderPass() = default;
};


#endif //METEOR_RENDERPASS_H
