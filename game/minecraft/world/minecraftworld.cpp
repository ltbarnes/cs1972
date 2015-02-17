#include "minecraftworld.h"
#include "point3d.h"
#include "mcchunkbuilder.h"
#include <QSet>
#include <QTime>

MinecraftWorld::MinecraftWorld(GLuint shader)
{
    m_vm = new VoxelManager(shader, Point(), Point(5, 2, 5), Point(32, 32, 32), new MCChunkBuilder(QTime::currentTime().msec()));
}


MinecraftWorld::~MinecraftWorld()
{
    delete m_vm;
}


void MinecraftWorld::onDraw(Graphics *g)
{
    g->setAtlas("terrain.png", glm::vec2(16.f));
    m_vm->onDraw(g);
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

