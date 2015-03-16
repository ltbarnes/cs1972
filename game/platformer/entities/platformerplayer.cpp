#include "platformerplayer.h"

PlatformerPlayer::PlatformerPlayer(ActionCamera *camera, glm::vec3 pos)
    : Player(camera, pos)
{
    m_offset = 5.f;
    m_camera->setOffset(m_offset);
}


PlatformerPlayer::~PlatformerPlayer()
{
}

void PlatformerPlayer::onTick(float secs)
{
    Player::onTick(secs);
}
