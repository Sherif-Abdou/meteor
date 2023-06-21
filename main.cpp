#include <iostream>
using std::string;

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <fstream>
#include <sstream>
#include "src/OBJFile.h"

string readFile(const char* path) {
    std::ifstream input_stream;
    std::stringstream strstream;
    input_stream.open(path);

    strstream << input_stream.rdbuf();
    string text = strstream.str();

    input_stream.close();
    return text;
}

unsigned int createShaderProgram(const char* vPath, const char* fPath) {
    unsigned int vertex_shader;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    string vFile = readFile(vPath);
    const char* vFileC = vFile.c_str();
    glShaderSource(vertex_shader, 1, &vFileC, nullptr);

    unsigned int fragment_shader;
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    string fFile = readFile(fPath);
    const char* fFileC = fFile.c_str();
    glShaderSource(fragment_shader, 1, &fFileC, nullptr);

    glCompileShader(vertex_shader);
    int success;
    char infoLog[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return shader_program;
}

unsigned int generateTriangleVAO(const std::vector<float>& buffer) {
    long len = buffer.size();

    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, len*sizeof(float), buffer.data(), GL_STATIC_DRAW);

    // Vertex
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Normal
    glVertexAttribPointer(1, 3, GL_FLOAT, false, 8*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    // Tex Coord
    glVertexAttribPointer(2, 2, GL_FLOAT, false, 8*sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);
    return vao;
}

OBJFile getOBJFromPath(string path) {
    std::ifstream stream(path);

    OBJFile file {stream};

    stream.close();

    return file;
}

void define_uniforms(unsigned int shader_program) {
    glm::mat4 modelViewMatrix = glm::mat4(1.0f);
    modelViewMatrix = glm::translate(modelViewMatrix, glm::vec3(0.0, 0.0, -3.0));
    modelViewMatrix = glm::rotate(modelViewMatrix, glm::radians(45.0f), glm::vec3(1.0, 0.0, 0.0));
    modelViewMatrix = glm::rotate(modelViewMatrix, glm::radians(45.0f), glm::vec3(0.0, 1.0, 0.0));
    int location = glGetUniformLocation(shader_program, "uModelView");
    glUniformMatrix4fv(location, 1, false, glm::value_ptr(modelViewMatrix));

    glm::mat4 perspectiveMatrix = glm::perspectiveFov(glm::radians(90.0f), 1920.0f,1080.0f, 0.1f, 100.0f);
    location = glGetUniformLocation(shader_program, "uPerspective");
    glUniformMatrix4fv(location, 1, false, glm::value_ptr(perspectiveMatrix));
    glm::vec3 eyePosition = glm::vec3(0, 0, 3.0);
    location = glGetUniformLocation(shader_program, "uEyePosition");
    glUniform3fv(location, 1, glm::value_ptr(eyePosition));
}

void render(unsigned int shader_program, long number, unsigned int vao) {
    glUseProgram(shader_program);

    define_uniforms(shader_program);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, number);
}

int main() {
    if (!glfwInit())
        return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(1920, 1080, "Meteor", nullptr, nullptr);

    glfwMakeContextCurrent(window);
    int version = gladLoadGL(glfwGetProcAddress);
    if (!version) {
        glfwTerminate();
        return -1;
    }
    glViewport(0,0,1920,1080);
    glEnable(GL_DEPTH_TEST);

    auto shader_program = createShaderProgram("shaders/vertex.vert", "shaders/fragment.frag");
    auto obj = getOBJFromPath("models/monkey.obj");
    auto buffer_data = obj.createBuffer();
    auto number = obj.size();
    auto vao = generateTriangleVAO(buffer_data);


    render(shader_program, number, vao);

    auto previous_time = glfwGetTime();
    auto current_time = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        current_time = glfwGetTime();
        double deltaTime = current_time - previous_time;
        if (current_time - previous_time > 1.0/60.0) {
            render(shader_program, number, vao);
            previous_time = current_time;
            glfwSwapBuffers(window);
        }
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
