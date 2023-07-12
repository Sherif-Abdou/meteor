//
// Created by sheri on 6/26/2023.
//

#ifndef METEOR_UNIFORMOBJECT_H
#define METEOR_UNIFORMOBJECT_H

#include <map>
#include <string>
#include <glm/glm.hpp>
#include "../ShaderProgram.h"
#include <glm/glm.hpp>

struct UniformObject {
public:
    std::map<std::string, glm::mat4> mat4Uniforms {};
    std::map<std::string, glm::vec3> vec3Uniforms {};
    std::map<std::string, int> intUniforms {};
    glm::mat4 projectionMatrix {};
    glm::mat4 viewMatrix {};
    void addToShader(ShaderProgram&);
};


#endif //METEOR_UNIFORMOBJECT_H
