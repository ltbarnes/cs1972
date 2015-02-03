#include "movableentity.h"

#include "printing.h"

MovableEntity::MovableEntity(glm::vec3 pos, float mass)
    : Entity(pos)
{
    m_mass = mass;
    m_vel = glm::vec3();

    m_force = glm::vec3();
    m_impulse = glm::vec3();
}

MovableEntity::~MovableEntity() {}

float MovableEntity::getMass()
{
    return m_mass;
}

glm::vec3 MovableEntity::getVelocity()
{
    return m_vel;
}

void MovableEntity::setMass(float mass)
{
    m_mass = mass;
}

void MovableEntity::setVelocity(glm::vec3 vel)
{
    m_vel = vel;
}

void MovableEntity::onTick(float secs)
{
    m_vel += (m_force * secs / m_mass) + m_impulse * 1.f / m_mass;
    setPosition(getPosition() + m_vel * secs);

    m_force = glm::vec3(0.f);
    m_impulse = glm::vec3(0.f);
}

void MovableEntity::handleCollision(Entity *, glm::vec3 mtv, glm::vec3 impulse)
{
    bump(mtv * .5f);
    applyImpulse(impulse);
}

void MovableEntity::applyImpulse(glm::vec3 impulse)
{
    m_impulse += impulse;
}

void MovableEntity::applyForce(glm::vec3 force)
{
    m_force += force;
}

