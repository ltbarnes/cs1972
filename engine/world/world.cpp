#include "world.h"

#include "printing.h"

World::World()
{
    m_staticEntities.clear();
    m_movableEntities.clear();
}

World::~World()
{
    foreach(Entity *e, m_staticEntities)
        delete e;
    foreach(Entity *e, m_movableEntities)
        delete e;
    foreach(Collision *c, m_collisions)
        delete c;
}

void World::addMovableEntity(MovableEntity *me)
{
    m_movableEntities.append(me);
}

void World::addStaticEntity(StaticEntity *se)
{
    m_staticEntities.append(se);
}

bool World::removeMovableEntity(MovableEntity *me)
{
    return m_movableEntities.removeOne(me);
}

bool World::removeStaticEntity(StaticEntity *se)
{
    return m_staticEntities.removeOne(se);
}

void World::onTick(float secs)
{
    // update (tick) movableEntities
    foreach(Entity *e, m_movableEntities)
    {
        e->onTick(secs);
    }

    // collisions
    detectCollisions();
    handleCollisions();
}

void World::detectCollisions()
{
    foreach(Collision *c, m_collisions)
        delete c;
    m_collisions.clear();

    QList<Collision *> collisions;
    Entity *e1, *e2;

    int moveSize = m_movableEntities.size();
    for (int i = 0; i < moveSize; i++)
    {
        e1 = m_movableEntities.value(i);

        // check static entities
        foreach(Entity *es, m_staticEntities)
        {
            collisions = e1->collides(es);
            if (collisions.size() > 0)
                m_collisions.append(collisions);
        }

        // check other movable entities
        for (int j = i + 1; j < moveSize; j++)
        {
            e2 = m_movableEntities.value(j);
            collisions = e1->collides(e2);
            if (collisions.size() > 0)
                m_collisions.append(collisions);
        }
    }
}

void World::handleCollisions()
{
    foreach(Collision *col, m_collisions)
    {
        col->e1->handleCollision(col->e2, col->mtv, col->impulse);
        col->e2->handleCollision(col->e1, col->mtv * -1.f, col->impulse * -1.f);
    }

}

void World::onDraw(Graphics *g)
{
    foreach(Entity *e, m_staticEntities)
        e->onDrawOpaque(g);

    foreach(Entity *e, m_movableEntities)
        e->onDrawOpaque(g);

    foreach(Entity *e, m_staticEntities)
        e->onDrawTransparent(g);

    foreach(Entity *e, m_movableEntities)
        e->onDrawTransparent(g);
}

