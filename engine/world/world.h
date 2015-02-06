#ifndef WORLD_H
#define WORLD_H

#include "movableentity.h"
#include "staticentity.h"
#include <QList>

class World
{
public:
    World();
    virtual ~World();

    void addMovableEntity(MovableEntity *me);
    void addStaticEntity(StaticEntity *se);
    bool removeMovableEntity(MovableEntity *me, bool clearMem);
    bool removeStaticEntity(StaticEntity *se, bool clearMem);

    void setToDeleteMovable(MovableEntity *me);

    virtual void onTick(float secs);
    virtual void onDraw(Graphics *g);

protected:
    virtual void detectCollisions();
    virtual void handleCollisions();

    QList<StaticEntity *> m_staticEntities;
    QList<MovableEntity *> m_movableEntities;

    QList<Collision *> m_collisions;
    QList<MovableEntity *> m_me2Delete;


};

#endif // WORLD_H
