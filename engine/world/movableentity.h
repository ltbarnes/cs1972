#ifndef MOVABLEENTITY_H
#define MOVABLEENTITY_H

#include "entity.h"

class MovableEntity : public Entity
{
public:
    MovableEntity(glm::vec3 pos = glm::vec3(0.f),
                  float mass = 1.f);
    virtual ~MovableEntity();

    virtual void onTick(float secs);

    virtual void handleCollision(Entity *other, glm::vec3 mtv, glm::vec3 impulse);

    virtual float getMass();
    virtual glm::vec3 getVelocity();

    void setMass(float mass);
    void applyImpulse(glm::vec3 impulse);
    void applyForce(glm::vec3 force);

protected:
    float m_mass;
    glm::vec3 m_vel, m_force, m_impulse;
};

#endif // MOVABLEENTITY_H
