#ifndef COLLISIONSHAPE_H
#define COLLISIONSHAPE_H

#include "entity.h"

class CollisionCylinder;

class CollisionShape
{
public:
    CollisionShape(glm::vec3 pos, glm::vec3 dim)
    {
        m_pos = pos;
        m_dim = dim;
    }
    virtual ~CollisionShape() {}

    void updatePos(glm::vec3 amount) { m_pos += amount; }

    glm::vec3 getPos() { return m_pos; }
    glm::vec3 getDim() { return m_dim; }

    virtual glm::vec4 collides(CollisionShape *shape) = 0;
    virtual glm::vec4 collidesCylinder(CollisionCylinder *cc) = 0;

protected:
    glm::vec3 m_pos, m_dim;

};

#endif // COLLISIONSHAPE_H
