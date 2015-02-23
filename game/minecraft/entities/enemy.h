#ifndef ENEMY_H
#define ENEMY_H

#include "movableentity.h"

class Enemy : public MovableEntity
{
public:
    Enemy(glm::vec3 pos);
    virtual ~Enemy();

    void onTick(float secs);

};

#endif // ENEMY_H
