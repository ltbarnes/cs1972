#include "enemy.h"
#include "collisioncylinder.h"

Enemy::Enemy(glm::vec3 pos)
    : MovableEntity(pos)
{
    CollisionShape *cs;
    cs = new CollisionCylinder(pos, glm::vec3(.99f, 1.98f, .99f), "enemy");
    cs->updatePos(pos);
    addCollisionShape(cs);

    RenderShape *rs;
    rs = new RenderShape();
    rs->type = CUBE;
    rs->color = glm::vec3(1, 0, 0);
    rs->trans = glm::scale(glm::mat4(), glm::vec3(.99f, 1.98f, .99f));
    rs->shininess = 0.f;
    rs->texture = "";
    rs->repeatU = 1.f;
    rs->repeatV = 1.f;
    addRenderShape(rs);
}


Enemy::~Enemy()
{
}


void Enemy::onTick(float )
{

}

