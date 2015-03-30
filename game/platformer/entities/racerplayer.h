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
    virtual void onKeyReleased(QKeyEvent *e);

    void setWaypoints(QList<glm::vec3> waypoints, glm::vec3 startLoc);

private:
    QList<glm::vec3> m_waypoints;
    int m_currWaypoint;
    bool m_lockCam;

};

#endif // RACERPLAYER_H
