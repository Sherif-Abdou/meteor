//
// Created by sheri on 6/24/2023.
//

#ifndef METEOR_GRAPHICSOBJECTFACTORY_H
#define METEOR_GRAPHICSOBJECTFACTORY_H

#include <string>
#include "GraphicsObject.h"


class GraphicsObjectFactory {
private:
    OBJFile getOBJFromPath(std::string);
public:
    GraphicsObject generateFromOBJ(std::string, ShaderProgram&);
    GraphicsObject* generateFromOBJRaw(std::string);
};


#endif //METEOR_GRAPHICSOBJECTFACTORY_H
