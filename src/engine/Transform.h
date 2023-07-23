//
// Created by sherif on 7/6/2023.
//

#ifndef METEOR_TRANSFORM_H
#define METEOR_TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <mutex>

class Transform {
private:
    Transform* parent = nullptr;
    std::mutex m;
public:
    Transform *getParent() const;

    void setParent(Transform *parent);

private:
    glm::vec3 position {};
public:
    glm::vec3 getPosition() const;

    void setPosition(const glm::vec3 &position);

    glm::vec3 getEulerAngles() const;

    void setEulerAngles(const glm::vec3 &eulerAngles);

    glm::vec3 getScale() const;

    void setScale(const glm::vec3 &scale);

private:
    glm::vec3 eulerAngles {};
    glm::vec3 scale = glm::vec3(1.0f);
    glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
public:
    glm::vec3 getRight() ;

    glm::vec3 getUp() ;

    glm::vec3 getForward();

private:
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 forward = glm::vec3(0.0f, 0.0f, -1.0f);

    glm::mat4 transform;
public:
    const glm::mat4 &getTransform() const;

private:
    glm::mat4 updateTransformMatrix();

    glm::mat4 getRotationMatrix();

    glm::mat4 calculateTransformMatrix() ;
};


#endif //METEOR_TRANSFORM_H
