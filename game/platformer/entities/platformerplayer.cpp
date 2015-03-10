#include "platformerplayer.h"

PlatformerPlayer::PlatformerPlayer(ActionCamera *camera, glm::vec3 pos)
    : Player(camera, pos)
{
}


PlatformerPlayer::~PlatformerPlayer()
{
}

void PlatformerPlayer::onTick(float secs)
{
    Player::onTick(secs);
}
