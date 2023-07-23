//
// Created by sherif on 6/24/2023.
//

#include <fstream>
#include "GraphicsObjectFactory.h"

GraphicsObject GraphicsObjectFactory::generateFromOBJ(std::string path, ShaderProgram& program) {
    auto obj = getOBJFromPath(path);
    return GraphicsObject {obj};
}

OBJFile GraphicsObjectFactory::getOBJFromPath(std::string path) {
    std::ifstream stream(path);

    OBJFile file {stream};

    stream.close();

    return file;
}

GraphicsObject *GraphicsObjectFactory::generateFromOBJRaw(std::string path) {
    auto obj = getOBJFromPath(std::move(path));
    return new GraphicsObject {obj};
}

