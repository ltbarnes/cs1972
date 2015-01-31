#include "movableentity.h"

#include "printing.h"

MovableEntity::MovableEntity(glm::vec3 pos, glm::vec3 dim, float mass)
    : Entity(pos, dim)
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

void MovableEntity::onTick(float secs)
{
    m_vel += (m_force * secs / m_mass) + m_impulse * 1.f / m_mass;
    m_pos += m_vel * secs;

    m_force = glm::vec3(0.f);
    m_impulse = glm::vec3(0.f);
}

void MovableEntity::handleCollision(Entity *other, glm::vec3 mtv, glm::vec3 impulse)
{

}

void MovableEntity::applyImpulse(glm::vec3 impulse)
{
    m_impulse += impulse;
}

void MovableEntity::applyForce(glm::vec3 force)
{
    m_force += force;
}

