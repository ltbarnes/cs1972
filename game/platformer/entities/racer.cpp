#include "racer.h"
#include "ellipsoid.h"

//#include <iostream>
//using namespace std;
//#include <glm/ext.hpp>

#include <glm/gtx/vector_angle.hpp>

Racer::Racer(glm::vec3 pos, glm::vec3 color)
    : MovableEntity(pos)
{
    setMass(1.f);

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

    m_currWaypoint = 0;
    m_finishedLap = false;
}


Racer::~Racer()
{
}


bool Racer::checkFinishedLap()
{
    bool result = m_finishedLap;
    m_finishedLap = false;
    return result;
}


void Racer::setWaypoints(QList<glm::vec3> waypoints, glm::vec3 startLoc)
{
    m_waypoints.clear();
    m_waypoints.append(waypoints);
    m_currWaypoint = 0;

    if (glm::dot(startLoc, startLoc) > 0.0001f)
    {
        glm::vec3 toWaypoint = startLoc - getPosition();
        float angle = glm::orientedAngle(glm::normalize(toWaypoint), glm::vec3(0, 0, -1), glm::vec3(0, -1, 0));
        m_rotation = glm::rotate(glm::mat4(), -angle, glm::vec3(0, -1, 0));
    }
}


void Racer::buildPath(NavMeshHandler *nmh)
{
    m_path.clear();
    if (m_waypoints.isEmpty())
        return;

    nmh->setStart(getPosition());
    nmh->setEnd(m_waypoints[m_currWaypoint]);
    nmh->findPath();
    m_path.append(nmh->getPath());
}


void Racer::onTick(float secs)
{

    float forceAmt = 20.f;
    if (m_waypoints.isEmpty())
        forceAmt = 0.f;

    glm::vec4 look = m_rotation * glm::vec4(0, 0, -1, 0);

    glm::vec3 thrust = glm::normalize(glm::vec3(look.x, 0.f, look.z));

    if (glm::length2(thrust) > 0.00001)
        thrust = glm::normalize(thrust) * forceAmt;


    glm::vec3 vel = (thrust - m_vel);
    vel.y = thrust.y;
    applyForce(vel);
    MovableEntity::onTick(secs);

    if (m_path.isEmpty())
        return;

    if (m_path.size() == 2)
        if (++m_currWaypoint >= m_waypoints.size())
        {
            m_currWaypoint = 0;
            m_finishedLap = true;
        }

    glm::vec3 toWaypoint = m_path[1] - getPosition();
    float angle = glm::orientedAngle(glm::normalize(toWaypoint), glm::vec3(0, 0, -1), glm::vec3(0, -1, 0));
    m_rotation = glm::rotate(glm::mat4(), -angle, glm::vec3(0, -1, 0));
}


void Racer::handleCollision(Collision *) {}


