#ifndef MINECRAFTWORLD_H
#define MINECRAFTWORLD_H

#include "world.h"
#include "voxelmanager.h"

class MinecraftWorld : public World
{
public:
    MinecraftWorld();
    virtual ~MinecraftWorld();

    void onDraw(Graphics *g);

    void addBlock(int x, int y, int z, char type);

private:
    VoxelManager *m_vm;
};

#endif // MINECRAFTWORLD_H
