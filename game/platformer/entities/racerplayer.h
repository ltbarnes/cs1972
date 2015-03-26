#ifndef RACERPLAYER_H
#define RACERPLAYER_H

#include "player.h"

class RacerPlayer : public Player
{
public:
    RacerPlayer(ActionCamera *camera, glm::vec3 pos, glm::vec3 color);
    virtual ~RacerPlayer();

    virtual void onTick(float secs);

    virtual void onKeyReleased(QKeyEvent *e);

private:
    bool m_lockCam;

};

#endif // RACERPLAYER_H
