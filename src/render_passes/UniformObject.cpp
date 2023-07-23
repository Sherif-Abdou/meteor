//
// Created by sherif on 6/26/2023.
//

#include "UniformObject.h"

void UniformObject::addToShader(ShaderProgram & shaderProgram) {
    for (auto [name, mat] : mat4Uniforms) {
        shaderProgram.addMatrix4Uniform(name, mat);
    }
    for (auto [name, vec] : vec3Uniforms) {
        shaderProgram.addVec3Uniform(name, vec);
    }
    for (auto [name, number] : intUniforms) {
        shaderProgram.addIntegerUniform(name, number);
    }
    shaderProgram.setPerspectiveMatrix(projectionMatrix);
    shaderProgram.setViewMatrix(viewMatrix);
    shaderProgram.addMatrix4Uniform("uNormalView", glm::inverseTranspose(viewMatrix));
}
