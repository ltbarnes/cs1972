#include "item.h"
#include "collisioncylinder.h"
#include "endgamescreen.h"
#include "application.h"

Item::Item(glm::vec3 pos, Player *player, Application *app)
    : MovableEntity(pos)
{
    m_player = player;
    m_app = app;

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
    glm::vec3 pos = getPosition();

    if (pos.y < -25.f)
        m_app->addScreen(new EndGameScreen(m_app, false));

    MovableEntity::onTick(secs);

    glm::vec3 playerPos = m_player->getPosition();

    glm::vec3 force = pos - playerPos;
    force.y = 0;
    applyForce(glm::normalize(force) * 10.f);
    applyForce(glm::vec3(-m_vel.x, 0.f, -m_vel.z) * 3.f); // "friction"
}
