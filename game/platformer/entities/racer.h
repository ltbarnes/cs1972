#ifndef RACER_H
#define RACER_H

#include "movableentity.h"

class Racer : public MovableEntity
{
public:
    Racer(glm::vec3 pos, glm::vec3 color);
    virtual ~Racer();
};

#endif // RACER_H
