#include "racerplayer.h"
#include "ellipsoid.h"

RacerPlayer::RacerPlayer(ActionCamera *camera, glm::vec3 pos, glm::vec3 color)
    : Player(camera, pos)
{
    m_offset = 10.f;
    m_camera->setOffset(m_offset);
    setEyeHeight(0.f);

    Ellipsoid *e = new Ellipsoid(glm::vec3(0, 0, 0), glm::vec3(.49f, .98f, .98f), "racer");
    e->updatePos(pos);
    this->addCollisionShape(e);

    RenderShape *rs;
    rs = new RenderShape();
    rs->type = SPHERE;
    rs->color = color;
    rs->shininess = 32.f;
    rs->transparency = 1.f;
    rs->trans = glm::scale(glm::mat4(), glm::vec3(.99f, .99f, 1.98f));
    rs->inv = glm::inverse(rs->trans);
    rs->texture = "";
    rs->repeatU = 1.f;
    rs->repeatV = 1.f;
    this->addRenderShape(rs);
}


RacerPlayer::~RacerPlayer()
{
}


