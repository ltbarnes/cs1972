#include "platformerplayer.h"
#include "ellipsoid.h"

PlatformerPlayer::PlatformerPlayer(ActionCamera *camera, glm::vec3 pos)
    : Player(camera, pos)
{
    m_offset = 0.f;
    m_camera->setOffset(m_offset);

    Ellipsoid *e = new Ellipsoid(glm::vec3(0, 0, 0), glm::vec3(.49f, .98f, .49f), "player");
    e->updatePos(pos);
    this->addCollisionShape(e);

    RenderShape *rs = new RenderShape();
    rs->type = SPHERE;
    rs->color = glm::vec3(0, 1.f, 0);
    rs->shininess = 32.f;
    rs->transparency = 1.f;
    rs->trans = glm::scale(glm::mat4(), glm::vec3(.99f, 1.98f, .99f));
    rs->texture = "";
    rs->repeatU = 1.f;
    rs->repeatV = 1.f;
    this->addRenderShape(rs);
}


PlatformerPlayer::~PlatformerPlayer()
{
}

void PlatformerPlayer::onTick(float secs)
{
    Player::onTick(secs);
}
