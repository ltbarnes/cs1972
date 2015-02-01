#ifndef STATICENTITY_H
#define STATICENTITY_H

#include "entity.h"

class StaticEntity : public Entity
{
public:
    StaticEntity(glm::vec3 pos = glm::vec3(0.f));
    virtual ~StaticEntity();

    virtual void handleCollision(Entity *other, glm::vec3 mtv, glm::vec3 impulse);
};

#endif // STATICENTITY_H
