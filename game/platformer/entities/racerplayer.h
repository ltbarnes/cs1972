#ifndef RACERPLAYER_H
#define RACERPLAYER_H

#include "player.h"

class RacerPlayer : public Player
{
public:
    RacerPlayer(ActionCamera *camera, glm::vec3 pos, glm::vec3 color);
    virtual ~RacerPlayer();

    virtual void onTick(float secs);
    virtual void onDrawTransparent(Graphics *g);

    virtual void onMouseMoved(QMouseEvent *e, float deltaX, float deltaY);
    virtual void onKeyPressed(QKeyEvent *e);
    virtual void onKeyReleased(QKeyEvent *e);

    bool checkFinishedLap();
    glm::vec3 getCurrentWaypoint();

    void setWaypoints(QList<glm::vec3> waypoints, glm::vec3 startLoc = glm::vec3());
    ObjectsInfo *getWaypointInfo();

private:
    QList<glm::vec3> m_waypoints;
    int m_currWaypoint;
    bool m_lockCam;
    bool m_finishedLap;

};

#endif // RACERPLAYER_H
