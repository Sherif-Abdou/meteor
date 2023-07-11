//
// Created by sheri on 7/7/2023.
//

#include "MeshComponent.h"
#include "../../GraphicsObjectFactory.h"

void MeshComponent::init() {
    GraphicsObjectFactory factory {};

    graphics_object = std::unique_ptr<GraphicsObject>(factory.generateFromOBJRaw(obj_path));
    if (albedoPath.has_value()) {
        graphics_object->setAlbedoTexture(*albedoPath);
    }
    if (normalPath.has_value()) {
        graphics_object->setNormalTexture(*normalPath);
    }

    pipeline.graphics_object[entity.getName()] = graphics_object.get();
}

void MeshComponent::update(float deltaTime) {
    graphics_object->translation = entity.transform.getPosition();
    graphics_object->rotation = entity.transform.getEulerAngles();
//    graphics_object->rotation *= glm::vec3(M_PI / 180.0f);
    graphics_object->scale = entity.transform.getScale();
}

const std::optional<std::string> &MeshComponent::getAlbedoPath() const {
    return albedoPath;
}

void MeshComponent::setAlbedoPath(const std::optional<std::string> &albedoPath) {
    MeshComponent::albedoPath = albedoPath;
}

const std::optional<std::string> &MeshComponent::getNormalPath() const {
    return normalPath;
}

void MeshComponent::setNormalPath(const std::optional<std::string> &normalPath) {
    MeshComponent::normalPath = normalPath;
}

const std::string &MeshComponent::getObjPath() const {
    return obj_path;
}

void MeshComponent::setObjPath(const std::string &objPath) {
    obj_path = objPath;
}

MeshComponent::MeshComponent(Entity & entity, Context & context, RenderPipeline & pipeline): Component(entity, context), pipeline(pipeline) {
}

MeshComponent::~MeshComponent() = default;

const char *MeshComponent::getComponentName() {
    return "mesh_component";
}

const char *MeshComponent::getClassComponentName() {
    return component_name;
}
