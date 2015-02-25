#include "enemy.h"
#include "collisioncylinder.h"

#include <glm/common.hpp>
#include <glm/ext.hpp>

#include <iostream>
using namespace std;

Enemy::Enemy(glm::vec3 pos)
    : MovableEntity(pos)
{

    CollisionShape *cs = new CollisionCylinder(glm::vec3(), glm::vec3(.99f, 1.98f, .99f), "enemy");
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

    m_lights.clear();
    m_stuckTime = 0.f;
    m_oldPos = pos;
}


Enemy::~Enemy()
{
}


void Enemy::addLight(glm::vec3 pos)
{
    m_lights.append(pos);
}


void Enemy::clearLights()
{
    m_lights.clear();
}


bool Enemy::isStuck()
{
    return m_stuckTime > 1.f;
}


void Enemy::onTick(float secs)
{
    glm::vec3 pos = getPosition();
    glm::vec3 dir = glm::vec3();

    if (!m_lights.isEmpty() && glm::length2(pos - m_oldPos) < 0.0001f)
        m_stuckTime += secs;
    else
        m_stuckTime = 0.f;

//    cout << !m_lights.isEmpty() << endl;
//    cout << glm::length2(pos - m_oldPos) << endl;
//    cout << m_stuckTime << endl;

    m_oldPos = pos;

    float dist2;
    float bestDist = std::numeric_limits<float>::infinity();
    foreach(glm::vec3 light, m_lights)
    {
        glm::vec3 diff = light - pos;
        dist2 = glm::dot(diff, diff);
        if (dist2 < bestDist)
        {
            bestDist = dist2;
            dir = diff;
        }
    }
    if (glm::length(dir) > 0.00001f)
        applyImpulse(glm::normalize(dir) * .1f);


    // movable entity stuff
    m_vel += (m_force * secs / m_mass) + m_impulse * 1.f / m_mass;

    m_force = glm::vec3(0.f);
    m_impulse = glm::vec3(0.f);
}


void Enemy::handleCollision(Collision *col)
{
    setPosition(getPosition() + col->mtv);
}

