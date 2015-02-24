#include "movableentity.h"
#include "collisionshape.h"

MovableEntity::MovableEntity(glm::vec3 pos)
    : Entity(pos)
{
    m_mass = 0.001f;
    m_vel = glm::vec3();

    m_force = glm::vec3();
    m_impulse = glm::vec3();
}

MovableEntity::~MovableEntity() {}

float MovableEntity::getMass()
{
    return m_mass;
}

void MovableEntity::addCollisionShape(CollisionShape *cs)
{
    Entity::addCollisionShape(cs);
    m_mass += cs->getMass();
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

void MovableEntity::handleCollision(Collision *col)
{
    if (!col->c1->isReactable() || !col->c2->isReactable())
        return;
    bump(col->mtv * .5f);
    applyImpulse(col->impulse);
}

void MovableEntity::applyImpulse(glm::vec3 impulse)
{
    m_impulse += impulse;
}

void MovableEntity::applyForce(glm::vec3 force)
{
    m_force += force;
}

