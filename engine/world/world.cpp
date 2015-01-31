#include "world.h"

World::World()
{
}

World::~World()
{
    foreach(Entity *e, m_staticEntities)
        delete e;
    foreach(Entity *e, m_movableEntities)
        delete e;
}

void World::addMovableEntity(MovableEntity *me)
{
    m_movableEntities.append(me);
}

void World::removeMovableEntity(MovableEntity *me)
{
    m_movableEntities.removeOne(me);
}

//void World::removeStaticEntity(StaticEntity *se)
//{
//    m_staticEntities.removeOne(e);
//}

void World::onTick(float secs)
{
    // update (tick) movableEntities
    foreach(Entity *e, m_movableEntities)
    {
        e->onTick(secs);
    }

    // collisions
    QList<Collision *> collisions = detectCollisions();
    handleCollisions(collisions);
}

QList<Collision *> World::detectCollisions()
{
    Collision *collision;
    QList<Collision *> collisions;
    Entity *e1, *e2;

    int moveSize = m_movableEntities.size();
    for (int i = 0; i < moveSize; i++)
    {
        e1 = m_movableEntities.value(i);

        // check static entities
        foreach(Entity *es, m_staticEntities)
        {
            collision = e1->collides(es);
            if (collision)
                collisions.append(collision);
        }

        // check other movable entities
        for (int j = i; j < moveSize; j++)
        {
            e2 = m_movableEntities.value(j);
            collision = e1->collides(e2);
            if (collision)
                collisions.append(collision);
        }
    }
    return collisions;
}

void World::handleCollisions(QList<Collision *> collisions)
{
    foreach(Collision *col, collisions)
    {
        col->e1->handleCollision(col->e2, col->mtv, col->impulse);
        col->e2->handleCollision(col->e1, col->mtv * -1.f, col->impulse * -1.f);
        delete col;
    }

}

void World::onDraw(Graphics *g)
{
    foreach(Entity *e, m_staticEntities)
        e->onDraw(g);

    foreach(Entity *e, m_movableEntities)
        e->onDraw(g);
}

