#include "minecraftworld.h"
#include "point3d.h"
#include "mcchunkbuilder.h"
#include <QSet>
#include <QTime>

#include <iostream>
using namespace std;

MinecraftWorld::MinecraftWorld(GLuint shader)
{
    m_vm = new VoxelManager(shader, Point(), Point(10, 2, 10), Point(32, 32, 32), new MCChunkBuilder(QTime::currentTime().msec()));

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

//    foreach (MovableEntity *me, m_movableEntities)
//        me->applyForce(glm::vec3(0, -10.f * me->getMass(), 0));
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


//void MinecraftWorld::addBlock(int x, int y, int z, char type)
//{
//    m_vm->addBlock(x, y, z, type);
//}

void MinecraftWorld::handleCollisions()
{
//    QHash<Entity *  , Collision *> mtvs;

    foreach(Collision *col, m_collisions)
    {
//        Entity *e = col->e2;
//        if (mtvs.contains(e))
//            col->mtv = glm::min(col->mtv, mtvs.value(e)->mtv);
        col->e2->handleCollision(col);
//        mtvs.insert(e, col);
    }

//    QList<Entity *> keys = mtvs.keys();
//    foreach(Entity *e, keys)
//        e->handleCollision(mtvs.value(e));

}
