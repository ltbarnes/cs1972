#include "collisioncylinder.h"

CollisionCylinder::CollisionCylinder(glm::vec3 pos, glm::vec3 dim)
    : CollisionShape(pos, dim)
{
}

CollisionCylinder::~CollisionCylinder()
{
}

Collision *CollisionCylinder::collides(CollisionShape *cs)
{
    return cs->collidesCylinder(this);
}

Collision *CollisionCylinder::collidesCylinder(CollisionCylinder *cc)
{
    glm::vec3 pos = cc->getPos();
    glm::vec3 dim = cc->getDim();

    float mtvMag;
    glm::vec3 mtvDir = glm::vec3(0.f);

    // check vertical
    float otherBelow = (pos.y + dim.y / 2.f) - (m_pos.y - m_dim.y / 2.f);
    float otherAbove = (m_pos.y + m_dim.y / 2.f) - (pos.y - dim.y / 2.f);

    if (otherBelow < 0.f || otherAbove < 0.f)
        mtvMag = std::numeric_limits<float>::infinity();
    else if (otherBelow < otherAbove)
    {
        mtvMag = otherBelow;
        mtvDir.y = -1.f;
    }
    else
    {
        mtvMag = otherAbove;
        mtvDir.y = 1.f;
    }

    // check xz plane



    return NULL;
}

