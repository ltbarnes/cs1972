#ifndef PLATFORMERPLAYER_H
#define PLATFORMERPLAYER_H

#include "player.h"

class PlatformerPlayer : public Player
{
public:
    PlatformerPlayer(ActionCamera *camera, glm::vec3 pos);
    virtual ~PlatformerPlayer();

    virtual void onTick(float secs);

};

#endif // PLATFORMERPLAYER_H
