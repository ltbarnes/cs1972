#include "minecraftworld.h"
#include "point3d.h"
#include <QSet>

#include <iostream>
using namespace std;

MinecraftWorld::MinecraftWorld(VoxelManager *vm)
{
    m_vm = vm;
    m_vm->setAtlas("terrain.png");
    addManager(m_vm);
}


MinecraftWorld::~MinecraftWorld()
{
//    World::~World(); // deletes m_vm
}

void MinecraftWorld::onTick(float secs)
{
    World::onTick(secs);

    foreach (MovableEntity *me, m_movableEntities)
        me->applyForce(glm::vec3(0, -11.f * me->getMass(), 0));
}

void MinecraftWorld::onDraw(Graphics *g)
{
    g->setGraphicsMode(DEFAULT);

    Light light;
    light.color = glm::vec3(1.f);
    light.pos = glm::vec3(-2, -1, 0);
    light.id = 1;

    g->setWorldColor(.1f, .4f, .1f);
    g->addLight(light);

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
