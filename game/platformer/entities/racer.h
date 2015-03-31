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

    bool checkFinishedLap();

    void setWaypoints(QList<glm::vec3> waypoints, glm::vec3 startLoc = glm::vec3());
    void buildPath(NavMeshHandler *nmh);

private:
    QList<glm::vec3> m_waypoints;
    QList<glm::vec3> m_path;
    int m_currWaypoint;
    bool m_finishedLap;

};

#endif // RACER_H
