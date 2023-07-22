//
// Created by sheri on 7/12/2023.
//

#include "Camera.h"

Transform &Camera::getTransform() {
    return transform;
}

float Camera::getFov() const {
    return fov;
}

void Camera::setCameraPosition(glm::vec3 position) {
    transform.setPosition(position);
    updatePipelineMatrices();
}

void Camera::setCameraRotation(glm::vec3 rotation) {
    transform.setEulerAngles(rotation);
    updatePipelineMatrices();
}

void Camera::updatePipelineMatrices() {
    for (auto pipeline : pipelines) {
        pipeline->setCameraTranslation(getTransform().getPosition());
        pipeline->setCameraRotation(-getTransform().getEulerAngles());
    }
}

Camera::Camera(std::vector<RenderPipeline*> pipelines) : pipelines(std::move(pipelines)) {

}

glm::vec3 Camera::getPosition() const {
    return transform.getPosition();
}

glm::vec3 Camera::getRotation() const {
    return transform.getEulerAngles();
}
