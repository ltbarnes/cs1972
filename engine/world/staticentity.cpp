#include "staticentity.h"
#include "movableentity.h"

StaticEntity::StaticEntity(glm::vec3 pos)
    : Entity(pos)
{
}

StaticEntity::~StaticEntity()
{
}


void StaticEntity::handleCollision(Entity *other, glm::vec3 mtv, glm::vec3)
{
    other->bump(mtv * -.5f);
    glm::vec3 momentum = other->getVelocity() * other->getMass();
    float mag2 = glm::dot(mtv, mtv);
    glm::vec3 imp;
    if (mag2 < 0.001f)
        imp = glm::vec3(0.f);
    else
        imp = (glm::dot(momentum, mtv) / mag2) * mtv;
    dynamic_cast<MovableEntity* >(other)->applyImpulse(-imp);
}

