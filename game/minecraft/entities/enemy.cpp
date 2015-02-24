#include "enemy.h"
#include "collisioncylinder.h"

Enemy::Enemy(glm::vec3 pos)
    : MovableEntity(pos)
{

    CollisionShape *cs = new CollisionCylinder(glm::vec3(), glm::vec3(.99f, 1.98f, .99f), "enemh");
    cs->updatePos(pos);
    addCollisionShape(cs);

    RenderShape *rs = new RenderShape();
    rs->type = CUBE;
    rs->color = glm::vec3(1, 0, 0);
    rs->shininess = 32.f;
    rs->transparency = 1.f;
    rs->trans = glm::scale(glm::mat4(), glm::vec3(.99f, 1.98f, .99f));
    rs->texture = "";
    rs->repeatU = 1.f;
    rs->repeatV = 1.f;

    addRenderShape(rs);
}


Enemy::~Enemy()
{
}


void Enemy::onTick(float secs)
{
    // movable entity stuff
    m_vel += (m_force * secs / m_mass) + m_impulse * 1.f / m_mass;

    m_force = glm::vec3(0.f);
    m_impulse = glm::vec3(0.f);
}


void Enemy::handleCollision(Collision *col)
{
    setPosition(getPosition() + col->mtv);
}

