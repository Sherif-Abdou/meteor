#include <iostream>
using std::string;

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <fstream>
#include "src/ShaderProgram.h"
#include "src/GraphicsObjectFactory.h"
#include "src/RenderPipeline.h"
#include "src/render_passes/ShadowPass.h"
#include "src/render_passes/GeometryPass.h"
#include "src/render_passes/DeferredPass.h"
#include "src/render_passes/SSAOPass.h"
#include "src/engine/CoreEngine.h"
#include "src/engine/components/MeshComponent.h"
#include "src/engine/components/WASDComponent.h"
#include "src/render_passes/SkyboxPass.h"
#include "src/render_passes/SSRPass.h"
#include "src/engine/components/HitboxComponent.h"

constexpr float WIDTH = 1920.0f;
constexpr float HEIGHT = 1080.0f;

const string MODEL = "models/monkey.obj";

const string MODEL2 = "models/background.obj";
const glm::vec3 eyePosition = glm::vec3(0.0, 0.0, 5.0f);

GLFWwindow * initialize_window() {
    if (!glfwInit())
        throw std::runtime_error("Can't Initialize Window");
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Meteor", nullptr, nullptr);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
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
    glfwSwapInterval(1);
    glEnable(GL_MULTISAMPLE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    return window;
}

int main() {
    auto window = initialize_window();

    auto shader_program = ShaderProgram("shaders/vertex.vert", "shaders/fragment.frag");
    auto debug_shader = ShaderProgram("shaders/debug.vert", "shaders/debug.frag");

    auto graphics_body_factory = GraphicsObjectFactory();

    auto deferred_quad_body = graphics_body_factory.generateFromOBJ("models/quad.obj", shader_program);
    auto skybox_cube_body = graphics_body_factory.generateFromOBJ("models/cube.obj", shader_program);


    shader_program.addVec3Uniform("uEyePosition", eyePosition);

    auto engine = CoreEngine();
    engine.setWindow(window);

    auto* shadowPass = new ShadowPass(glm::vec3(0.0f, 4.0f, -0.0f));
    auto* geoPass = new GeometryPass();
    auto* ssaoPass = new SSAOPass();
    auto* ssrPass = new SSRPass();
    ssrPass->width /= 2.0f;
    ssrPass->height /= 2.0f;
//    ssaoPass->width /= 2.0f;
//    ssaoPass->height /= 2.0f;
    auto* deferredPass = new DeferredPass();
    auto* skyboxPass = new SkyboxPass();
    std::string str = "textures/new_sky.jpg";
    std::string arr[6] = {str, str, str, str, str, str};
    skyboxPass->setSkyboxPaths(arr);
    engine.pipeline.uniforms.projectionMatrix = glm::perspectiveFov(glm::radians(90.f), 1920.0f, 1080.0f, 0.1f, 100.0f);
    engine.getCamera()->setCameraPosition({0.0f, 0.0f, -7.0f});

    engine.pipeline.graphics_object["deferred_quad"] = &deferred_quad_body;
    engine.pipeline.graphics_object["skybox_cube"] = &skybox_cube_body;


    engine.pipeline.addPass(shadowPass);
    engine.pipeline.addPass(geoPass);
    engine.pipeline.addPass(ssrPass);
    engine.pipeline.addPass(ssaoPass);
    engine.pipeline.addPass(deferredPass);
    engine.pipeline.addPass(skyboxPass);

    auto& main_object = engine.createEntityFromOBJPath("main", MODEL);
    auto& second_object = engine.createEntityFromOBJPath("second", MODEL2);
    main_object.addComponent(std::make_unique<WASDComponent>(main_object, engine.getContext()));
    main_object.getComponent<MeshComponent>().setAlbedoPath("textures/1001_albedo.jpg");
//    main_object.getComponent<MeshComponent>().setNormalPath("textures/1001_normal.png");
    main_object.getComponent<HitboxComponent>().height *= 2.0f;
    main_object.transform.setPosition(glm::vec3(0.0, 1.0, -0.0));
    second_object.getComponent<MeshComponent>().setAlbedoPath("textures/red.jpg");
    second_object.transform.setPosition(second_object.transform.getPosition() + glm::vec3(0.0, -3.0, 0.0));
    second_object.transform.setScale(glm::vec3(5.0f));

    engine.frame_init();
    engine.start_physics();
    engine.frame_loop();
//    glfwSwapBuffers(window);

    engine.end_physics();
    glfwTerminate();
    delete shadowPass;
    delete geoPass;
    delete ssaoPass;
    delete deferredPass;
    return 0;
}
