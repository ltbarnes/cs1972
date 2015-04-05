#include "collisionmanager.h"

CollisionManager::CollisionManager()
{
}


CollisionManager::~CollisionManager()
{
}


void CollisionManager::manage(World *world, float onTickSecs)
{
    // collisions
    QList<Collision *> cols = detectCollisions(world, onTickSecs);
    handleCollisions(cols);

        foreach (Collision * col, cols)
            delete col;
}

QList<Collision *> CollisionManager::detectCollisions(World *world, float secs)
{
    QList<Collision *> cols;

    QList<Collision *> collisions;
    Entity *e1, *e2;

    QList<MovableEntity *> mes = world->getMovableEntities();
    QList<StaticEntity *> ses = world->getStaticEntities();

    int moveSize = mes.size();
    for (int i = 0; i < moveSize; i++)
    {
        e1 = mes[i];

        // check static entities
        foreach(Entity *es, ses)
        {
            collisions = es->collides(e1, secs);
            if (collisions.size() > 0)
                cols.append(collisions);
        }

        // check other movable entities
        for (int j = i + 1; j < moveSize; j++)
        {
            e2 = mes[j];
            collisions = e1->collides(e2, secs);
            if (collisions.size() > 0)
                cols.append(collisions);
        }
    }
    return cols;
}

void CollisionManager::handleCollisions(QList<Collision* > cols)
{
    foreach(Collision *col, cols)
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


