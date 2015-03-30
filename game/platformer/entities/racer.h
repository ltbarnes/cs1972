#ifndef RACER_H
#define RACER_H

#include "movableentity.h"
#include "navmeshhandler.h"

class Racer : public MovableEntity
{
public:
    Racer(glm::vec3 pos, glm::vec3 color);
    virtual ~Racer();

    virtual void onTick(float secs);
    virtual void handleCollision(Collision *col);

    void setWaypoints(QList<glm::vec3> waypoints, glm::vec3 startLoc);
    void buildPath(NavMeshHandler *nmh);

private:
    QList<glm::vec3> m_waypoints;
    QList<glm::vec3> m_path;
    int m_currWaypoint;

};

#endif // RACER_H
