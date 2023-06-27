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
#include "src/Renderer.h"
#include "src/ShadowMap.h"
#include "src/GraphicsObjectFactory.h"
#include "src/RenderPipeline.h"
#include "src/render_passes/ForwardPass.h"
#include "src/render_passes/ShadowPass.h"
#include "src/render_passes/GeometryPass.h"
#include "src/render_passes/DeferredPass.h"
#include "src/render_passes/SSAOPass.h"

constexpr float WIDTH = 1920.0f;
constexpr float HEIGHT = 1080.0f;

const string MODEL = "models/super_backpack.obj";

const string MODEL2 = "models/background.obj";
const glm::vec3 eyePosition = glm::vec3(0.0, 0.0, 5.0f);

void setup_camera(ShaderProgram &shader_program);
void frame_loop(GLFWwindow *window, glm::vec3 velocity, RenderPipeline& pipeline);


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

GLFWwindow * initialize_window() {
    if (!glfwInit())
        throw std::runtime_error("Can't Initialize Window");
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Meteor", nullptr, nullptr);

    glfwMakeContextCurrent(window);
    int version = gladLoadGL(glfwGetProcAddress);
    if (!version) {
        glfwTerminate();
        throw std::runtime_error("Can't Initialize GL");
    }
    glViewport(0,0,WIDTH,HEIGHT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_FRAMEBUFFER_SRGB);
    glEnable(GL_CULL_FACE);
    glfwWindowHint(GL_SAMPLES, 4);
    glEnable(GL_MULTISAMPLE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    return window;
}

int main() {
    auto window = initialize_window();

    auto shader_program = ShaderProgram("shaders/vertex.vert", "shaders/fragment.frag");
    auto debug_shader = ShaderProgram("shaders/debug.vert", "shaders/debug.frag");

    auto graphics_body_factory = GraphicsObjectFactory();

    auto graphics_body = graphics_body_factory.generateFromOBJ(MODEL, shader_program);
    auto graphics_body2 = graphics_body_factory.generateFromOBJ(MODEL2, shader_program);
    auto graphics_body3 = graphics_body_factory.generateFromOBJ("models/quad.obj", shader_program);

    setup_camera(shader_program);

    shader_program.addVec3Uniform("uEyePosition", eyePosition);

    graphics_body.translation = glm::vec3(0.0f, -0.0f, 0.0f);
    graphics_body.rotation = glm::vec3(0.0f, -45.f, 45.0f);
    graphics_body.scale = glm::vec3(0.3f);
    graphics_body2.translation = glm::vec3(0.0, -1.0f, 0.0f);
//
//    // Slapped together gravity
//    glm::vec3 velocity = glm::vec3(0.5f, 0.5f, 0.0f);
//    auto shadow_shaders = ShaderProgram("shaders/light.vert", "shaders/light.frag");
//
//    auto renderer = Renderer();
//    renderer.shader_programs.push_back(std::move(shader_program));
//    renderer.shader_programs.push_back(std::move(shadow_shaders));
//    renderer.objects.push_back(std::move(graphics_body));
//    renderer.objects.push_back(std::move(graphics_body2));
//
//    auto shadow_map = ShadowMap(renderer.shader_programs[1]);
//    shadow_map.light_position = glm::vec3(0.0f, 6.0f, -0.0f);
//    shadow_map.render_depth_map(renderer);
//    renderer.objects[0].shader_program.addMatrix4Uniform("uLightSpaceMatrix", shadow_map.getLightSpaceMatrix());
//    renderer.render_to_window();
//    frame_loop(window, velocity, renderer, shadow_map);
    RenderPipeline pipeline {};

    RenderPass* shadowPass = new ShadowPass(glm::vec3(0.0f, 4.0f, -0.0f));
    RenderPass* geoPass = new GeometryPass();
    RenderPass* ssaoPass = new SSAOPass();
    RenderPass* deferredPass = new DeferredPass();

    pipeline.graphics_object["main"] = &graphics_body;
    pipeline.graphics_object["main"]->setAlbedoTexture("textures/1001_albedo.jpg");
    pipeline.graphics_object["second"] = &graphics_body2;
    pipeline.graphics_object["second"]->setAlbedoTexture("textures/red.jpg");
    pipeline.graphics_object["deferred_quad"] = &graphics_body3;
    pipeline.addPass(shadowPass);
    pipeline.addPass(geoPass);
    pipeline.addPass(ssaoPass);
    pipeline.addPass(deferredPass);

    pipeline.init();
    pipeline.render();
    glfwSwapBuffers(window);

    frame_loop(window, glm::vec3(0.0f, 1.0f, 0.0f), pipeline);

    glfwTerminate();
    delete shadowPass;
    delete geoPass;
    delete ssaoPass;
    delete deferredPass;
    return 0;
}

void frame_loop(GLFWwindow *window, glm::vec3 velocity, RenderPipeline& pipeline) {
    int frame_counter = 0;
    auto previous_time = glfwGetTime();
    double current_time;
    auto last_second = glfwGetTime();
    bool motion = false;
    bool held = false;
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        if (glfwGetKey(window, GLFW_KEY_SPACE)) {
            if (!held) {
                motion = !motion;
                held = true;
            }
        } else {
            held = false;
        }
        current_time = glfwGetTime();
        double deltaTime = current_time - previous_time;
        if (current_time - previous_time > 0.0) {
            if (motion) {
                velocity -= glm::vec3(0.0f * deltaTime, 0.2f * deltaTime, 0.0f * deltaTime);
//                pipeline.graphics_object["main"]->translation += glm::vec3(deltaTime) * velocity;
                pipeline.graphics_object["main"]->rotation.y += 45.0f * deltaTime;
                pipeline.render();
            }

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
}

void setup_camera(ShaderProgram &shader_program) {
    glm::mat4 modelViewMatrix = glm::mat4(1.0f);
    modelViewMatrix = glm::translate(modelViewMatrix, glm::vec3(0.0, 0.0, -5.));
    glm::mat4 perspectiveMatrix = glm::perspectiveFov(glm::radians(90.0f), WIDTH,HEIGHT, 0.1f, 100.0f);

    shader_program.setPerspectiveMatrix(perspectiveMatrix);
    shader_program.setViewMatrix(modelViewMatrix);
}
