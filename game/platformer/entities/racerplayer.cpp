#include "racerplayer.h"
#include "ellipsoid.h"

#include <glm/gtx/norm.hpp>

RacerPlayer::RacerPlayer(ActionCamera *camera, glm::vec3 pos, glm::vec3 color)
    : Player(camera, pos)
{
    m_offset = 25.f;
    m_camera->setOffset(m_offset);
    setEyeHeight(0.f);
    m_lockCam = false;

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


void RacerPlayer::onTick(float secs)
{
    float forceAmt = 30.f;
    glm::vec3 force = glm::vec3();
    if (m_wsad & 0b1000)
        force.z += 1;
    if (m_wsad & 0b0100)
        force.z -= 1;
    if (m_wsad & 0b0010)
        force.x -= 1;
    if (m_wsad & 0b0001)
        force.x += 1;


    glm::vec4 look = m_rotation * glm::vec4(0, 0, -1, 0);
    if (m_lockCam)
    {
        look = glm::normalize(glm::vec4(look.x, 0, look.z, 0));
        look.y = -.1f;
        m_camera->setLook(glm::normalize(look));
    }

    glm::vec3 thrust = glm::normalize(glm::vec3(look.x, 0.f, look.z)) * force.z;
//    thrust += glm::normalize(glm::vec3(-look.z, 0.f, look.x)) * force.x;
    if (glm::length2(thrust) > 0.00001)
        thrust = glm::normalize(thrust) * forceAmt;


    glm::vec3 vel = (thrust - m_vel);
    vel.y = thrust.y;
    applyForce(vel);
    MovableEntity::onTick(secs);
    if (force.z < 0.f)
        force.x = -force.x;

    m_rotation *= glm::rotate(glm::mat4(), glm::radians(force.x), glm::vec3(0, -1, 0));
}


void RacerPlayer::onKeyReleased(QKeyEvent *e)
{
    switch (e->key())
    {
    case Qt::Key_8:
        m_lockCam = !m_lockCam;
        break;
    default:
        Player::onKeyReleased(e);
        break;
    }
}


