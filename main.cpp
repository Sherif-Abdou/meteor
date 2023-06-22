#include <iostream>
using std::string;

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <fstream>
#include <sstream>
#include "src/OBJFile.h"
#include "src/ShaderProgram.h"
#include "src/GraphicsObject.h"

constexpr float WIDTH = 800.0f;
constexpr float HEIGHT = 600.0f;

const string MODEL = "models/different_sphere.obj";

const string MODEL2 = "models/floor.obj";

string readFile(const char* path) {
    std::ifstream input_stream;
    std::stringstream strstream;
    input_stream.open(path);

    strstream << input_stream.rdbuf();
    string text = strstream.str();

    input_stream.close();
    return text;
}

OBJFile getOBJFromPath(string path) {
    std::ifstream stream(path);

    OBJFile file {stream};

    stream.close();

    return file;
}

int main() {
    if (!glfwInit())
        return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Meteor", nullptr, nullptr);

    glfwMakeContextCurrent(window);
    int version = gladLoadGL(glfwGetProcAddress);
    if (!version) {
        glfwTerminate();
        return -1;
    }
    glViewport(0,0,WIDTH,HEIGHT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_FRAMEBUFFER_SRGB);
    glEnable(GL_CULL_FACE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto obj = getOBJFromPath(MODEL);
    auto shader_program = ShaderProgram("shaders/vertex.vert", "shaders/fragment.frag");
    auto graphics_body = GraphicsObject(obj, shader_program);

    obj = getOBJFromPath(MODEL2);
    auto graphics_body2 = GraphicsObject(obj, shader_program);

    glm::mat4 modelViewMatrix = glm::mat4(1.0f);
    modelViewMatrix = glm::translate(modelViewMatrix, glm::vec3(0.0, 0.0, -4.5));
    glm::mat4 perspectiveMatrix = glm::perspectiveFov(glm::radians(90.0f), WIDTH,HEIGHT, 0.1f, 100.0f);

    shader_program.setPerspectiveMatrix(perspectiveMatrix);
    shader_program.setViewMatrix(modelViewMatrix);
    auto eyePosition = glm::vec3(0.0, 0.0, 4.5f);
    shader_program.addVec3Uniform("uEyePosition", eyePosition);

    auto previous_time = glfwGetTime();
    double current_time;
    auto last_second = glfwGetTime();
    int frame_counter = 0;
    graphics_body.translation = glm::vec3(-0.5f, 0.5f, 0.0f);

    auto velocity = glm::vec3(0.5f, 0.5f, 0.0f);
    bool start = true;
    graphics_body2.translation = glm::vec3(0.0, -3.0f, 0.0f);
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        current_time = glfwGetTime();
        double deltaTime = current_time - previous_time;
        if (current_time - previous_time > 1.0/300.0 && start) {
            velocity -= glm::vec3(0.0f * deltaTime, 0.2f * deltaTime, 0.0f * deltaTime);
            graphics_body.translation += glm::vec3(velocity.x*deltaTime, velocity.y * deltaTime, velocity.z * deltaTime);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            graphics_body.render();
            graphics_body2.render();

            previous_time = current_time;
            glfwSwapBuffers(window);
            frame_counter++;
        }
        if (glfwGetTime() - last_second > 1.0) {
            std::cout << "FPS: " << frame_counter << "\n";
            last_second = glfwGetTime();
            frame_counter = 0;
        }
    }
    glfwTerminate();
    return 0;
}
