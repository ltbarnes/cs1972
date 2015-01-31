#ifndef WORLD_H
#define WORLD_H

#include "entity.h"
#include "movableentity.h"
#include <QList>

class World
{
public:
    World();
    ~World();

    void addMovableEntity(MovableEntity *me);
    void removeMovableEntity(MovableEntity *me);
//    void addStaticEntity(StaticEntity *se);
//    void removeStaticEntity(StaticEntity *se);

    virtual void onTick(float secs);
    virtual void onDraw(Graphics *g);

protected:
//    virtual void updateCamera(Graphics *g) = 0;
    virtual QList<Collision *> detectCollisions();
    virtual void handleCollisions(QList<Collision *> collisions);

    QList<Entity *> m_staticEntities;
    QList<MovableEntity *> m_movableEntities;


};

#endif // WORLD_H
