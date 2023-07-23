//
// Created by sherif on 6/21/2023.
//

#include "ShaderProgram.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::string __readFile(const char* path) {
    std::ifstream input_stream;
    std::stringstream strstream;
    input_stream.open(path);

    strstream << input_stream.rdbuf();
    std::string text = strstream.str();

    input_stream.close();
    return text;
}

ShaderProgram::ShaderProgram(const ShaderProgram::string& vertexShaderPath, const ShaderProgram::string& fragmentShaderPath) {
    unsigned int vertex_shader;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    string vFile = __readFile(vertexShaderPath.c_str());
    const char* vFileC = vFile.c_str();
    glShaderSource(vertex_shader, 1, &vFileC, nullptr);

    unsigned int fragment_shader;
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    string fFile = __readFile(fragmentShaderPath.c_str());
    const char* fFileC = fFile.c_str();
    glShaderSource(fragment_shader, 1, &fFileC, nullptr);

    glCompileShader(vertex_shader);
    int success;
    char infoLog[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAG::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    shader_program_id = shader_program;
}

void ShaderProgram::addGlobalUniforms() {
    auto location = glGetUniformLocation(shader_program_id, "uPerspective");
    glUniformMatrix4fv(location, 1, false, glm::value_ptr(perspectiveMatrix));

    location = glGetUniformLocation(shader_program_id, "uModelView");
    glUniformMatrix4fv(location, 1, false, glm::value_ptr(viewMatrix));
}

void ShaderProgram::clearLocalUniforms() {
    localMatrix4Uniforms.clear();
    localVec3Uniforms.clear();
    localIntegerUniforms.clear();
}

void ShaderProgram::addMatrix4Uniform(const ShaderProgram::string &name, glm::mat4 matrix) {
    localMatrix4Uniforms[name] = matrix;
}

void ShaderProgram::addVec3Uniform(const ShaderProgram::string &name, glm::vec3 vector) {
    localVec3Uniforms[name] = vector;
}

void ShaderProgram::addLocalUniforms() {
    int location;
    for (const auto& [name, matrix] : localMatrix4Uniforms) {
        location = glGetUniformLocation(shader_program_id, name.c_str());
        glUniformMatrix4fv(location, 1, false, glm::value_ptr(matrix));
    }

    for (const auto& [name, vector] : localVec3Uniforms) {
        location = glGetUniformLocation(shader_program_id, name.c_str());
        glUniform3fv(location, 1, glm::value_ptr(vector));
    }

    for (const auto& [name, vector] : localVec4Uniforms) {
        location = glGetUniformLocation(shader_program_id, name.c_str());
        glUniform4fv(location, 1, glm::value_ptr(vector));
    }

    for (const auto& [name, integer] : localIntegerUniforms) {
        location = glGetUniformLocation(shader_program_id, name.c_str());
        glUniform1i(location, integer);
    }
}

void ShaderProgram::addUniforms() {
    addGlobalUniforms();
    addLocalUniforms();
}

void ShaderProgram::applyProgram() {
    glUseProgram(shader_program_id);
}

const glm::mat4 &ShaderProgram::getPerspectiveMatrix() const {
    return perspectiveMatrix;
}

void ShaderProgram::setPerspectiveMatrix(const glm::mat4 &perspectiveMatrix) {
    ShaderProgram::perspectiveMatrix = perspectiveMatrix;
}

const glm::mat4 &ShaderProgram::getViewMatrix() const {
    return viewMatrix;
}

void ShaderProgram::setViewMatrix(const glm::mat4 &viewMatrix) {
    ShaderProgram::viewMatrix = viewMatrix;
}

void ShaderProgram::addIntegerUniform(const ShaderProgram::string &name, int number) {
    localIntegerUniforms[name] = number;
}

unsigned int ShaderProgram::getShaderProgramId() const {
    return shader_program_id;
}

void ShaderProgram::addVec4Uniform(const ShaderProgram::string &name, glm::vec4 vector) {
    localVec4Uniforms[name] = vector;
}
