#include "mccollisionmanager.h"
#include "world.h"
#include "collisionshape.h"
#include "player.h"

#include <iostream>
using namespace std;

MCCollisionManager::MCCollisionManager()
{
}


MCCollisionManager::~MCCollisionManager()
{
}


QList<Collision *> MCCollisionManager::detectCollisions(World *world, float secs)
{
    QList<Collision *> cols, colsTemp;

    QList<MovableEntity *> mes = world->getMovableEntities();

    int num = mes.size();
    MovableEntity *me1, *me2;
    for (int i = 0; i < num - 1; i++)
    {
        me1 = mes.value(i);
        for (int j = i + 1; j < num; j++)
        {
            me2 = mes.value(j);
            colsTemp = me1->collides(me2, secs);
            if (colsTemp.size() > 0)
                cols.append(colsTemp);
        }
    }

    return cols;
}


void MCCollisionManager::handleCollisions(QList<Collision* > cols)
{
    foreach (Collision *c, cols)
    {
        if (c->c1->getID() == "player")
            dynamic_cast<Player*>(c->e1)->decreaseHealth(51);
    }
}


