#ifndef MCCOLLISIONMANAGER_H
#define MCCOLLISIONMANAGER_H

#include "collisionmanager.h"

class MCCollisionManager : public CollisionManager
{
public:
    MCCollisionManager();
    virtual ~MCCollisionManager();

    virtual QList<Collision *> detectCollisions(World *world, float secs);
    virtual void handleCollisions(QList<Collision* > cols);
};

#endif // MCCOLLISIONMANAGER_H
