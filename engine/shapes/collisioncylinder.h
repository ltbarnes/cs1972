#ifndef COLLISIONCYLINDER_H
#define COLLISIONCYLINDER_H

#include "collisionshape.h"
#include "entity.h"

class CollisionCylinder : public CollisionShape
{
public:
    CollisionCylinder(glm::vec3 pos, glm::vec3 dim);
    virtual ~CollisionCylinder();

    virtual glm::vec4 collides(CollisionShape *cs);
    virtual glm::vec4 collidesCylinder(CollisionCylinder *cc);
};

#endif // COLLISIONCYLINDER_H
