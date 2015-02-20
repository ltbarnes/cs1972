#ifndef MINECRAFTWORLD_H
#define MINECRAFTWORLD_H

#include "world.h"
#include "voxelmanager.h"

class MinecraftWorld : public World
{
public:
    MinecraftWorld(Camera *cam, VoxelManager *vm);
    virtual ~MinecraftWorld();

    virtual void onDraw(Graphics *g);
    virtual void onTick(float secs);

//    void addBlock(int x, int y, int z, char type);

    inline static glm::vec4 getCoords(int index, Point dim);

private:
    VoxelManager *m_vm;
    glm::mat4 m_selectedFace;

    Camera *m_camera;
};

#endif // MINECRAFTWORLD_H
