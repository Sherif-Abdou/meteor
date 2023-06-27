cmake_minimum_required(VERSION 3.20)
project(meteor)

set(CMAKE_CXX_STANDARD 17)

include_directories("glad/include")
include_directories("include")

add_executable(meteor main.cpp glad/src/gl.c src/OBJFile.cpp src/OBJFile.h src/GraphicsObject.cpp src/GraphicsObject.h src/ShaderProgram.cpp src/ShaderProgram.h src/ShadowMap.cpp src/ShadowMap.h src/Renderer.cpp src/Renderer.h src/GraphicsObjectFactory.cpp src/GraphicsObjectFactory.h src/RenderPipeline.cpp src/RenderPipeline.h src/render_passes/RenderPass.cpp src/render_passes/RenderPass.h src/render_passes/ForwardPass.cpp src/render_passes/ForwardPass.h src/render_passes/ShadowPass.cpp src/render_passes/ShadowPass.h src/render_passes/UniformObject.cpp src/render_passes/UniformObject.h src/render_passes/GeometryPass.cpp src/render_passes/GeometryPass.h src/render_passes/DeferredPass.cpp src/render_passes/DeferredPass.h src/render_passes/SSAOPass.cpp src/render_passes/SSAOPass.h)
target_link_libraries(meteor GL)
target_link_libraries(meteor glfw)
#target_link_libraries(meteor glm)
