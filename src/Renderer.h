//
// Created by sherif on 6/22/2023.
//

#ifndef METEOR_RENDERER_H
#define METEOR_RENDERER_H

#include <vector>
#include <optional>
#include "ShaderProgram.h"
#include "GraphicsObject.h"

class Renderer {
public:
    Renderer() = default;
    explicit Renderer(const Renderer&) = delete;
    Renderer(Renderer&&) = default;
    std::vector<ShaderProgram> shader_programs {};
    std::vector<GraphicsObject> objects {};
    void render_to_window();
    void raw_render(bool custom_shader = false);
    void raw_render(ShaderProgram&);
};


#endif //METEOR_RENDERER_H
