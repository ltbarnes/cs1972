#ifndef RACERPLAYER_H
#define RACERPLAYER_H

#include "player.h"

class RacerPlayer : public Player
{
public:
    RacerPlayer(ActionCamera *camera, glm::vec3 pos, glm::vec3 color);
    virtual ~RacerPlayer();
};

#endif // RACERPLAYER_H
