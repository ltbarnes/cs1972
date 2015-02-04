#include "item.h"
#include "collisioncylinder.h"

Item::Item(glm::vec3 pos, Player *player)
    : MovableEntity(pos)
{
    m_player = player;

    CollisionShape *cs;
    cs = new CollisionCylinder(glm::vec3(), glm::vec3(1.f, 1.f, 1.f), "item");
    addCollisionShape(cs);

    RenderShape *rs;
    rs = new RenderShape();
    rs->type = CYLINDER;
    rs->color = glm::vec3(.5f, .8f, .5f);
    rs->shininess = 0.f;
    rs->transparency = 1.f;
    rs->trans = glm::mat4();
    rs->texture = "";
    rs->repeatU = 1.f;
    rs->repeatV = 1.f;

    addRenderShape(rs);
}

Item::~Item()
{
}


void Item::onTick(float secs)
{
    MovableEntity::onTick(secs);

    applyForce(glm::vec3(-m_vel.x, 0.f, -m_vel.z) * 3.f); // "friction"
}
