#ifndef WORLD_H
#define WORLD_H

#include "movableentity.h"
#include "staticentity.h"
#include "player.h"
#include <QList>

class Manager;

class World
{
public:
    World();
    virtual ~World();

    void addMovableEntity(MovableEntity *me);
    void addStaticEntity(StaticEntity *se);
    bool removeMovableEntity(MovableEntity *me, bool clearMem);
    bool removeStaticEntity(StaticEntity *se, bool clearMem);

    QList<MovableEntity *> getMovableEntities();
    QList<StaticEntity *> getStaticEntities();

    void setToDeleteMovable(MovableEntity *me);

    virtual void onTick(float secs);
    virtual void onDraw(Graphics *g);

    void addManager(Manager *m);

protected:
    Player *m_player;

    QList<StaticEntity *> m_staticEntities;
    QList<MovableEntity *> m_movableEntities;

    QList<Collision *> m_collisions;
    QList<MovableEntity *> m_me2Delete;

    QList<Manager *> m_managers;
};

#endif // WORLD_H
