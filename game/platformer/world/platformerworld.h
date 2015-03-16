#ifndef PLATFORMERWORLD_H
#define PLATFORMERWORLD_H

#include "world.h"

class PlatformerWorld : public World
{
public:
    PlatformerWorld();
    virtual ~PlatformerWorld();

    Triangle *intersectWorld(glm::vec3 p, glm::vec3 d, float *t);
};

#endif // PLATFORMERWORLD_H
