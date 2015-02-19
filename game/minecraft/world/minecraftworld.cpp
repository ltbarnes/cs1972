#include "minecraftworld.h"
#include "point3d.h"
#include "mcchunkbuilder.h"
#include <QSet>
#include <QTime>

#include <iostream>
using namespace std;

MinecraftWorld::MinecraftWorld(GLuint shader)
{
    m_vm = new VoxelManager(shader, Point(), Point(5, 1, 5), Point(32, 32, 32), new MCChunkBuilder(QTime::currentTime().msec()));

//    QList<Chunk *> chunks = m_vm->getChunks();

//    cout << "Chunks: " << chunks.size() << endl;

//    foreach (Chunk *chunk, chunks)
//        addStaticEntity(chunk);
}


MinecraftWorld::~MinecraftWorld()
{
    delete m_vm;
}

void MinecraftWorld::onTick(float secs)
{
    World::onTick(secs);

    foreach (MovableEntity *me, m_movableEntities)
        me->applyForce(glm::vec3(0, -11.f * me->getMass(), 0));
}

void MinecraftWorld::onDraw(Graphics *g)
{
    g->setAtlas("terrain.png", glm::vec2(16.f));
    m_vm->onDraw(g);

    g->setGraphicsMode(DEFAULT);
    World::onDraw(g);
}

glm::vec4 MinecraftWorld::getCoords(int index, Point dim)
{
    return glm::vec4(((index / dim.y) % dim.x),
                     (index % dim.y),
                     (index / (dim.x*dim.y)),
                     0.f);
}

void MinecraftWorld::detectCollisions(float secs)
{
    foreach(Collision *c, m_collisions)
        delete c;
    m_collisions.clear();

    Collision *col;
    MovableEntity *me;

    int moveSize = m_movableEntities.size();
    for (int i = 0; i < moveSize; i++)
    {
        me = m_movableEntities.value(i);

        col = m_vm->predictCollision(me, secs);

        if (col)
            m_collisions.append(col);
    }
}

void MinecraftWorld::handleCollisions()
{
    foreach(Collision *col, m_collisions)
    {
        col->e1->handleCollision(col);
    }
}

//void MinecraftWorld::predictCollision(MovableEntity *me, float secs)
//{
//    m_vm->predictCollision(me, secs);
//}


//void MinecraftWorld::addBlock(int x, int y, int z, char type)
//{
//    m_vm->addBlock(x, y, z, type);
//}
