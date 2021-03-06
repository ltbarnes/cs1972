#include "racerplayer.h"
#include "ellipsoid.h"
#include "collisioncylinder.h"

#include <glm/gtx/norm.hpp>
#include <glm/gtx/vector_angle.hpp>

RacerPlayer::RacerPlayer(ActionCamera *camera, glm::vec3 pos, glm::vec3 color)
    : Player(camera, pos)
{
    m_offset = 15.f;
    m_camera->setOffset(m_offset);
    setEyeHeight(0.f);
    m_lockCam = true;

    m_waypointRadius = 10.f;

    Ellipsoid *e = new Ellipsoid(glm::vec3(0), glm::vec3(.98f), "racer");
    e->updatePos(pos);
    this->addCollisionShape(e);

    CollisionCylinder *c = new CollisionCylinder(glm::vec3(0), glm::vec3(1.98f), "racer");
    c->updatePos(pos);
    this->addCollisionShape(c);

    RenderShape *rs;
    rs = new RenderShape();
    rs->type = SPHERE;
    rs->color = color;
    rs->shininess = 32.f;
    rs->transparency = 1.f;
    rs->trans = glm::scale(glm::mat4(), glm::vec3(2, .5, 2));
    rs->inv = glm::inverse(rs->trans);
    rs->texture = "";
    rs->repeatU = 1.f;
    rs->repeatV = 1.f;
    this->addRenderShape(rs);

    rs = new RenderShape();
    rs->type = CONE;
    rs->color = color;
    rs->shininess = 32.f;
    rs->transparency = 1.f;
    rs->trans = glm::translate(glm::mat4(), glm::vec3(.3, .2, .6))
              * glm::scale(glm::mat4(), glm::vec3(.5f))
              * glm::rotate(glm::mat4(), glm::half_pi<float>(), glm::vec3(-1, 0, 0));
    rs->inv = glm::inverse(rs->trans);
    rs->texture = "";
    rs->repeatU = 1.f;
    rs->repeatV = 1.f;
    this->addRenderShape(rs);

    rs = new RenderShape();
    rs->type = CONE;
    rs->color = color;
    rs->shininess = 32.f;
    rs->transparency = 1.f;
    rs->trans = glm::translate(glm::mat4(), glm::vec3(-.3, .2, .6))
            * glm::scale(glm::mat4(), glm::vec3(.5f))
            * glm::rotate(glm::mat4(), glm::half_pi<float>(), glm::vec3(-1, 0, 0));
    rs->inv = glm::inverse(rs->trans);
    rs->texture = "";
    rs->repeatU = 1.f;
    rs->repeatV = 1.f;
    this->addRenderShape(rs);

    rs = new RenderShape();
    rs->type = SPHERE;
    rs->color = glm::vec3(1);
    rs->shininess = 32.f;
    rs->transparency = 1.f;
    rs->trans = glm::translate(glm::mat4(), glm::vec3(0, .2, -.5))
              * glm::scale(glm::mat4(), glm::vec3(.5,.4,.5));
    rs->inv = glm::inverse(rs->trans);
    rs->texture = "";
    rs->repeatU = 1.f;
    rs->repeatV = 1.f;
    this->addRenderShape(rs);

    m_currWaypoint = 0;
    m_finishedLap = false;
}


RacerPlayer::~RacerPlayer()
{
}


bool RacerPlayer::checkFinishedLap()
{
    bool result = m_finishedLap;
    m_finishedLap = false;
    return result;
}


glm::vec3 RacerPlayer::getCurrentWaypoint()
{
    if (m_waypoints.isEmpty())
        return glm::vec3();

    return m_waypoints[m_currWaypoint];
}


float RacerPlayer::getWaypointRadius()
{
    return m_waypointRadius;
}


void RacerPlayer::onTick(float secs)
{
    float forceAmt = 25.f;
    glm::vec3 force = glm::vec3();
    if (m_wsad & 0b1000)
        force.z += 1;
    if (m_wsad & 0b0100)
        force.z = 0;
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

    if (glm::length2(thrust) > 0.00001)
        thrust = glm::normalize(thrust) * forceAmt;


    glm::vec3 vel = (thrust - m_vel);
    vel.y = thrust.y;
    applyForce(vel * 5.f);
    MovableEntity::onTick(secs);
    if (glm::dot(getVelocity(), glm::vec3(look)) < 0.f && force.z < 0.f)
        force.x = -force.x;

    m_rotation *= glm::rotate(glm::mat4(), glm::radians(force.x), glm::vec3(0, -1, 0));

    if (!m_waypoints.isEmpty())
    {
        if (glm::distance2(getPosition(), m_waypoints[m_currWaypoint]) < 100.f)
        {
            if (++m_currWaypoint >= m_waypoints.size())
            {
                m_currWaypoint = 0;
                m_finishedLap = true;
            }
        }
    }
}


void RacerPlayer::onDrawTransparent(Graphics *g)
{
    Player::onDrawTransparent(g);

    if (!m_waypoints.isEmpty())
    {
        glm::mat4 trans;
        g->setTransparentMode(true);

        if (m_waypoints.size() > 1)
        {
            int next = (m_currWaypoint + 1) % m_waypoints.size();

            trans = glm::translate(glm::mat4(), m_waypoints[next]) *
                    glm::scale(glm::mat4(), glm::vec3(m_waypointRadius * .8f));
            g->setColor(1, 0, 1, .2, 0);
            g->drawSphere(trans);

            trans = glm::translate(glm::mat4(), m_waypoints[m_currWaypoint]) *
                    glm::scale(glm::mat4(), glm::vec3(m_waypointRadius * 1.9f));
            g->setColor(0, 1, 1, .3, 0);
        }
        else
        {
            trans = glm::translate(glm::mat4(), m_waypoints[m_currWaypoint]) *
                    glm::scale(glm::mat4(), glm::vec3(m_waypointRadius * 2.2f));
            g->setColor(0, 1, 0, .3, 0);
        }
        g->drawSphere(trans);

        g->setTransparentMode(false);
    }
}


void RacerPlayer::onMouseMoved(QMouseEvent *e, float deltaX, float deltaY)
{
    Player::onMouseMoved(e, deltaX, deltaY);
    if (m_camera->getLook().y > 0.f)
        m_camera->pitch(-deltaY * .1f);
}


void RacerPlayer::onKeyPressed(QKeyEvent *e)
{
    switch (e->key())
    {
    case Qt::Key_W:
        m_wsad |= 0b1000;
        break;
    case Qt::Key_S:
        m_wsad |= 0b0100;
        break;
    case Qt::Key_A:
        m_wsad |= 0b0010;
        break;
    case Qt::Key_D:
        m_wsad |= 0b0001;
        break;
    case Qt::Key_Space:
        m_jump = true;
        break;
    case Qt::Key_Minus:
    case Qt::Key_Underscore:
        m_offset += 1.f;
        if (m_offset > 35.f)
            m_offset = 35.f;
        m_camera->setOffset(m_offset);
        break;
    case Qt::Key_Plus:
    case Qt::Key_Equal:
        m_offset -= 1.f;
        if (m_offset < 0.f)
            m_offset = 0.f;
        m_camera->setOffset(m_offset);
        break;
    case Qt::Key_ParenRight:
    case Qt::Key_0:
        m_offset = 0.f;
        m_camera->setOffset(m_offset);
        break;
    case Qt::Key_ParenLeft:
    case Qt::Key_9:
        m_offset = 35.f;
        m_camera->setOffset(m_offset);
        break;

    default:
        break;
    }
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


void RacerPlayer::setWaypoints(QList<glm::vec3> waypoints, glm::vec3 startLoc)
{
    m_waypoints.clear();
    m_waypoints.append(waypoints);
    m_currWaypoint = 0;

    if (glm::dot(startLoc, startLoc) > 0.0001f)
    {
        glm::vec3 toWaypoint = startLoc - getPosition();
        float angle = glm::orientedAngle(glm::normalize(toWaypoint), glm::vec3(0, 0, -1), glm::vec3(0, -1, 0));
        m_rotation = glm::rotate(glm::mat4(), -angle, glm::vec3(0, -1, 0));

        glm::vec4 look = m_rotation * glm::vec4(0, 0, -1, 0);

        look = glm::normalize(glm::vec4(look.x, 0, look.z, 0));
        look.y = -.1f;
        m_camera->setLook(glm::normalize(look));

        this->setCameraPos();
    }
}


ObjectsInfo *RacerPlayer::getWaypointInfo()
{
    ObjectsInfo *info;
    info = new ObjectsInfo();

    glm::mat4 trans;
    glm::vec4 color;
    if (m_waypoints.size() > 1)
    {
        int next = (m_currWaypoint + 1) % m_waypoints.size();

        trans = glm::translate(glm::mat4(), m_waypoints[next]) *
                glm::scale(glm::mat4(), glm::vec3(m_waypointRadius * .8f));
        info->invs.append(glm::inverse(trans));
        info->colors.append(glm::vec4(1, 0, 1, .2));
        info->shapeType.append(SPHERE);

        trans = glm::translate(glm::mat4(), m_waypoints[m_currWaypoint]) *
                glm::scale(glm::mat4(), glm::vec3(m_waypointRadius * 1.9f));
        color = glm::vec4(0, 1, 1, .3);
    }
    else
    {
        trans = glm::translate(glm::mat4(), m_waypoints[m_currWaypoint]) *
                glm::scale(glm::mat4(), glm::vec3(m_waypointRadius * 2.2f));
        color = glm::vec4(0, 1, 0, .3);
    }
    info->invs.append(glm::inverse(trans));
    info->colors.append(color);
    info->shapeType.append(SPHERE);

    return info;
}


