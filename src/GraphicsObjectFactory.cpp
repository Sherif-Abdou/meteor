//
// Created by sheri on 6/24/2023.
//

#include <fstream>
#include "GraphicsObjectFactory.h"

GraphicsObject GraphicsObjectFactory::generateFromOBJ(std::string path, ShaderProgram& program) {
    auto obj = getOBJFromPath(path);
    return GraphicsObject {obj, program};
}

OBJFile GraphicsObjectFactory::getOBJFromPath(std::string path) {
    std::ifstream stream(path);

    OBJFile file {stream};

    stream.close();

    return file;
}

