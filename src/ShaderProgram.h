//
// Created by sheri on 6/21/2023.
//

#ifndef METEOR_SHADERPROGRAM_H
#define METEOR_SHADERPROGRAM_H

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <string>
#include <map>
#include <unordered_map>

class ShaderProgram {
private:
    using string = std::string;
    std::map<int, int> textures;
    glm::mat4 perspectiveMatrix = glm::identity<glm::mat4>();
    glm::mat4 viewMatrix = glm::identity<glm::mat4>(); // Transform objects for camera purposes
    std::map<string, glm::mat4> localMatrix4Uniforms {};
public:
    const glm::mat4 &getPerspectiveMatrix() const;

    void setPerspectiveMatrix(const glm::mat4 &perspectiveMatrix);

    const glm::mat4 &getViewMatrix() const;

    void setViewMatrix(const glm::mat4 &viewMatrix);

private:
    std::map<string, glm::vec3> localVec3Uniforms {};
    std::map<string, int> localIntegerUniforms {};
    unsigned int shader_program_id;
    void addGlobalUniforms();
    void addLocalUniforms();
public:
    explicit ShaderProgram(const string& vertexShaderPath, const string& fragmentShaderPath);
    explicit ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram(ShaderProgram&&) = default;
    ShaderProgram& operator=(ShaderProgram&&) = default;
    void clearLocalUniforms();
    void addMatrix4Uniform(const string& name, glm::mat4 matrix);
    void addVec3Uniform(const string& name, glm::vec3 vector);
    void addIntegerUniform(const string& name, int number);
    void addUniforms();
    void applyProgram();
};


#endif //METEOR_SHADERPROGRAM_H
