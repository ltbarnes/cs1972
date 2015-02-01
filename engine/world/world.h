#ifndef WORLD_H
#define WORLD_H

#include "entity.h"
#include "movableentity.h"
#include "staticentity.h"
#include <QList>

class World
{
public:
    World();
    virtual ~World();

    void addMovableEntity(MovableEntity *me);
    void removeMovableEntity(MovableEntity *me);
    void addStaticEntity(StaticEntity *se);
    void removeStaticEntity(StaticEntity *se);

    virtual void onTick(float secs);
    virtual void onDraw(Graphics *g);

protected:
    virtual void detectCollisions();
    virtual void handleCollisions();

    QList<StaticEntity *> m_staticEntities;
    QList<MovableEntity *> m_movableEntities;

    QList<Collision *> m_collisions;


};

#endif // WORLD_H
