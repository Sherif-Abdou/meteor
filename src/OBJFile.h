//
// Created by sheri on 6/20/2023.
//

#ifndef METEOR_OBJFILE_H
#define METEOR_OBJFILE_H

#include <glm/glm.hpp>
#include <vector>
#include <sstream>

struct Face {
    std::vector<glm::vec3> vertices {};
    std::vector<glm::vec3> normals {};
    std::vector<glm::vec2> texCoords {};
};

class OBJFile {
private:
    using string = std::string;
    std::vector<glm::vec3> vertices{};
    std::vector<glm::vec3> normals{};
    std::vector<glm::vec2> texCoords{};
    std::vector<Face> faces{};
public:
    explicit OBJFile(std::istream&);
    std::vector<float> createBuffer();
    long size() const;
};


#endif //METEOR_OBJFILE_H
