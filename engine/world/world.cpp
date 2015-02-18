#include "world.h"
#include "voxelmanager.h"

World::World()
{
    m_staticEntities.clear();
    m_movableEntities.clear();
    m_collisions.clear();
    m_me2Delete.clear();
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

bool World::removeMovableEntity(MovableEntity *me, bool clearMem)
{
    bool result = m_movableEntities.removeOne(me);
    if (clearMem)
        delete me;
    return result;
}

void World::setToDeleteMovable(MovableEntity *me)
{
    m_me2Delete.append(me);
}

bool World::removeStaticEntity(StaticEntity *se, bool clearMem)
{
    bool result = m_staticEntities.removeOne(se);
    if (clearMem)
        delete se;
    return result;
}

void World::onTick(float secs)
{
    foreach(MovableEntity *me, m_me2Delete)
    {
        removeMovableEntity(me, true);
    }
    m_me2Delete.clear();

    // update (tick) movableEntities
    foreach(Entity *e, m_movableEntities)
    {
        e->onTick(secs);
    }

    // collisions
    detectCollisions(secs);
    handleCollisions();
}

void World::detectCollisions(float secs)
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
            collisions = es->collides(e1, secs);
            if (collisions.size() > 0)
                m_collisions.append(collisions);
        }

        // check other movable entities
        for (int j = i + 1; j < moveSize; j++)
        {
            e2 = m_movableEntities.value(j);
            collisions = e1->collides(e2, secs);
            if (collisions.size() > 0)
                m_collisions.append(collisions);
        }
    }
}

void World::handleCollisions()
{
    foreach(Collision *col, m_collisions)
    {
        col->e1->handleCollision(col);

        // swap
        Entity *tempE = col->e1;
        col->e1 = col->e2;
        col->e2 = tempE;
        CollisionShape *tempS = col->c1;
        col->c1 = col->c2;
        col->c2 = tempS;
        col->mtv *= -1.f;
        col->impulse *= -1.f;

        col->e1->handleCollision(col);
    }

}

void World::onDraw(Graphics *g)
{
//    if (m_vm)
//        m_vm->onDraw(g);

    foreach(Entity *e, m_staticEntities)
        e->onDrawOpaque(g);

    foreach(Entity *e, m_movableEntities)
        e->onDrawOpaque(g);

    foreach(Entity *e, m_staticEntities)
        e->onDrawTransparent(g);

    foreach(Entity *e, m_movableEntities)
        e->onDrawTransparent(g);
}

//void World::addManager(VoxelManager *vm)
//{

//}

