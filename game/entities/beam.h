#ifndef BEAM_H
#define BEAM_H

#include "staticentity.h"

class Beam : public StaticEntity
{
public:
    Beam(glm::vec3 pos);
    virtual ~Beam();

    virtual void handleCollision(Entity *other, glm::vec3 mtv, glm::vec3 impulse);

};

#endif // BEAM_H
