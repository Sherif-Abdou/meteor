//
// Created by sherif on 7/7/2023.
//

#ifndef METEOR_MESHCOMPONENT_H
#define METEOR_MESHCOMPONENT_H


#include "../Component.h"
#include "../../GraphicsObject.h"
#include "../../RenderPipeline.h"
#include <memory>

class MeshComponent: public Component {
private:
    std::unique_ptr<GraphicsObject> graphics_object;
    std::optional<std::string> albedoPath;
    std::optional<std::string> normalPath;
    glm::vec4 solid_albedo;
    std::string obj_path;
    RenderPipeline& pipeline;
public:
    constexpr static const char* component_name = "mesh_component";

    [[nodiscard]] const std::optional<std::string> &getAlbedoPath() const;

    void setAlbedoPath(const std::optional<std::string> &albedoPath);

    [[nodiscard]] const std::optional<std::string> &getNormalPath() const;

    void setNormalPath(const std::optional<std::string> &normalPath);

    [[nodiscard]] const std::string &getObjPath() const;

    void setObjPath(const std::string &objPath);

    void init() override;


    void update(float deltaTime) override;
    explicit MeshComponent(Entity&, Context&, RenderPipeline&);
    ~MeshComponent() override;

    const char *getComponentName() override;
    static const char *getClassComponentName();

    glm::vec4 getSolidColor();
    void setSolidColor(const glm::vec4&);
};


#endif //METEOR_MESHCOMPONENT_H
