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
#include "src/render_passes/ForwardPass.h"
#include "src/render_passes/WeightedBlendPass.h"

constexpr float WIDTH = 1920.0f;
constexpr float HEIGHT = 1080.0f;

const string MODEL = "models/train.obj";
const string MODEL2 = "models/background.obj";
const string MODEL3 = "models/monkey.obj";
const string MODEL4 = "models/super_backpack.obj";

const glm::vec3 eyePosition = glm::vec3(0.0, 0.0, 5.0f);


GLFWwindow * initialize_window() {
    if (!glfwInit())
        throw std::runtime_error("Can't Initialize Window");
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#ifndef NDEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);
    std::cout << "Debug context enabled\n";
#endif
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
//    glfwWindowHint(GL_SAMPLES, 4);
    glfwSwapInterval(1);
//    glEnable(GL_MULTISAMPLE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    return window;
}

static unsigned int* gFBO;
RenderPipeline generateDeferredPipeline() {
    RenderPipeline pipeline {};

    auto* shadowPass = new ShadowPass(glm::vec3(0.0f, 4.0f, -0.0f));
    auto* geoPass = new GeometryPass();
    auto* ssaoPass = new SSAOPass();
    auto* ssrPass = new SSRPass();
    auto* deferredPass = new DeferredPass();
    auto* skyboxPass = new SkyboxPass();
    std::string str = "textures/new_sky.jpg";
    std::string arr[6] = {str, str, str, str, str, str};
    skyboxPass->setSkyboxPaths(arr);

    pipeline.addPass(shadowPass);
    pipeline.addPass(geoPass);
    pipeline.addPass(ssaoPass);
    pipeline.addPass(ssrPass);
    pipeline.addPass(deferredPass);
    pipeline.addPass(skyboxPass);
    gFBO = geoPass->getFBOPointer();

    return pipeline;
}

RenderPipeline generateTransparentForwardPipeline() {
    RenderPipeline pipeline {};
    auto* forwardPass = new ForwardPass();
    forwardPass->transparencyMode = true;
    forwardPass->pipeline = &pipeline;
    forwardPass->depth_buffer_ptr = gFBO;

    pipeline.addPass(forwardPass);

    return pipeline;
}



int main() {
    auto window = initialize_window();

    auto shader_program = ShaderProgram("shaders/vertex.vert", "shaders/fragment.frag");
    auto debug_shader = ShaderProgram("shaders/debug.vert", "shaders/debug.frag");

    auto graphics_body_factory = GraphicsObjectFactory();

    auto deferred_quad_body = graphics_body_factory.generateFromOBJ("models/quad.obj", shader_program);
    auto skybox_cube_body = graphics_body_factory.generateFromOBJ("models/cube.obj", shader_program);


    shader_program.addVec3Uniform("uEyePosition", eyePosition);

    auto pipeline = generateDeferredPipeline();
    auto transparency_pipeline = generateTransparentForwardPipeline();
    auto engine = CoreEngine(pipeline, transparency_pipeline);
    engine.setWindow(window);

    engine.pipeline.uniforms.projectionMatrix = glm::perspectiveFov(glm::radians(90.f), 1920.0f, 1080.0f, 0.1f, 100.0f);
    engine.transparent_pipeline.uniforms.projectionMatrix = glm::perspectiveFov(glm::radians(90.f), 1920.0f, 1080.0f, 0.1f, 100.0f);
    engine.getCamera()->setCameraPosition({0.0f, 0.0f, -7.0f});
    engine.pipeline.graphics_object["deferred_quad"] = &deferred_quad_body;
    engine.pipeline.graphics_object["skybox_cube"] = &skybox_cube_body;


    auto& main_object = engine.createEntityFromOBJPath("main", MODEL);
    main_object.addComponent(std::make_unique<WASDComponent>(main_object, engine.getContext()));
    main_object.addComponent(std::make_unique<RigidBodyComponent>(main_object, engine.getContext()));
    main_object.getComponent<RigidBodyComponent>().setAcceleration(glm::vec3 {-0.0f, -1.f, 0.0f});
    main_object.getComponent<MeshComponent>().setAlbedoPath("textures/locomotive_diffuse.png");

    main_object.getComponent<HitboxComponent>().height *= 1.0f;
    main_object.getComponent<HitboxComponent>().offset.y += 0.5f;
    main_object.transform.setPosition(glm::vec3 {-3.3f, -0.f, 3.00f});

    auto& second_object = engine.createEntityFromOBJPath("second", MODEL2);
    second_object.addComponent(std::make_unique<RigidBodyComponent>(second_object, engine.getContext()));
    second_object.getComponent<RigidBodyComponent>().setMass(8000.0f);
    second_object.getComponent<HitboxComponent>().width *= 5.0f;
    second_object.getComponent<HitboxComponent>().depth *= 5.0f;
    second_object.getComponent<MeshComponent>().setSolidColor({0.3, 0.4, 0.5, 1.0});
    second_object.transform.setPosition(second_object.transform.getPosition() + glm::vec3(0.0, -3.0, 0.0));
    second_object.transform.setScale(glm::vec3(5.0f));

//    auto& third_object = engine.createEntityFromOBJPath("third", MODEL4);
//    third_object.addComponent(std::make_unique<RigidBodyComponent>(third_object, engine.getContext()));
//    third_object.getComponent<MeshComponent>().setSolidColor(glm::vec4(0.0, 0.1, 0.5, 1.0));


    auto& transparent_object = engine.createTransparentEntityFromOBJPath("transparent", MODEL3);
    transparent_object.transform.setPosition({1, 1, 2});

    engine.frame_init();
    engine.start_physics();
    engine.frame_loop();
//    glfwSwapBuffers(window);

    engine.end_physics();
    glfwTerminate();
//    delete shadowPass;
//    delete geoPass;
//    delete ssaoPass;
//    delete deferredPass;
//    delete ssrPass;
//    delete skyboxPass;
    return 0;
}