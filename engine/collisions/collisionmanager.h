#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include "manager.h"
#include "world.h"
#include <QList>

class CollisionManager : public Manager
{
public:
    CollisionManager();
    virtual ~CollisionManager();

    virtual void manage(World *world, float onTickSecs);

    virtual QList<Collision *> detectCollisions(World *world, float secs);
    virtual void handleCollisions(QList<Collision* > cols);
};

#endif // COLLISIONMANAGER_H
