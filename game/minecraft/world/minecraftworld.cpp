#include "minecraftworld.h"

MinecraftWorld::MinecraftWorld()
{
    m_vm = new VoxelManager(Point(), Point(1, 1, 1), Point(32, 32, 32));
}


MinecraftWorld::~MinecraftWorld()
{
    delete m_vm;
}


void MinecraftWorld::onDraw(Graphics *g)
{
    m_vm->onDraw(g);
}


void MinecraftWorld::addBlock(int x, int y, int z, char type)
{
    m_vm->addBlock(x, y, z, type);
}

