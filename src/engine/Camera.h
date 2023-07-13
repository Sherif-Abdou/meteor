//
// Created by sheri on 7/12/2023.
//

#ifndef METEOR_CAMERA_H
#define METEOR_CAMERA_H


#include "Transform.h"
#include "../RenderPipeline.h"

class Camera {
private:
    Transform transform;
    RenderPipeline& pipeline;
    float fov = 90.0f;
    void updatePipelineMatrices();
public:
    explicit Camera(RenderPipeline &pipeline);

    float getFov() const;
    const Transform &getTransform() const;
    glm::vec3 getPosition() const;
    glm::vec3 getRotation() const;
    void setCameraPosition(glm::vec3 position);
    void setCameraRotation(glm::vec3 rotation);
};


#endif //METEOR_CAMERA_H
