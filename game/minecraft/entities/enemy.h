#ifndef ENEMY_H
#define ENEMY_H

#include "movableentity.h"

class Enemy : public MovableEntity
{
public:
    Enemy(glm::vec3 pos);
    virtual ~Enemy();

    virtual void onTick(float secs);
    virtual void handleCollision(Collision *col);

};

#endif // ENEMY_H
