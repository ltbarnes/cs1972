#include "platformerworld.h"

#include <iostream>
using namespace std;

PlatformerWorld::PlatformerWorld()
{
}


PlatformerWorld::~PlatformerWorld()
{
}


Triangle *PlatformerWorld::intersectWorld(glm::vec3 p, glm::vec3 d, float *t)
{
    QList<Triangle *> tris = this->getMesh();
    Triangle *triangle = NULL;
    *t = INFINITY;
    float t2;

    foreach (Triangle *tri, tris)
    {
        t2 = tri->intersectPlane(p, d);
        if (t2 < *t)
        {
            *t = t2;
            triangle = tri;
        }
    }

    return triangle;
}

