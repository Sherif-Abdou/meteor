//
// Created by sheri on 7/6/2023.
//

#ifndef METEOR_TRANSFORM_H
#define METEOR_TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>

class Transform {
private:
    glm::vec3 position;
public:
    const glm::vec3 &getPosition() const;

    void setPosition(const glm::vec3 &position);

    const glm::vec3 &getEulerAngles() const;

    void setEulerAngles(const glm::vec3 &eulerAngles);

    const glm::vec3 &getScale() const;

    void setScale(const glm::vec3 &scale);

private:
    glm::vec3 eulerAngles;
    glm::vec3 scale = glm::vec3(1.0f);

    glm::mat4 transform;
public:
    const glm::mat4 &getTransform() const;

private:
    glm::mat4 updateTransformMatrix();
};


#endif //METEOR_TRANSFORM_H