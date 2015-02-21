#include "minecraftworld.h"
#include "point3d.h"
#include <QSet>

#include <iostream>
using namespace std;

MinecraftWorld::MinecraftWorld(Camera *cam, VoxelManager *vm)
{
    m_vm = vm;
    m_vm->setAtlas("terrain.png");
    addManager(m_vm);

    m_camera = cam;
    m_selectedFace = glm::mat4();
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

    float t;
    int face;
    glm::vec3 point = m_vm->castRay(glm::vec3(m_camera->getEye()), glm::vec3(m_camera->getLook()), t, face);

    switch(face) {
    case 0b100000: // +z
        m_selectedFace = glm::mat4();
        m_selectedFace = glm::scale(glm::mat4(), glm::vec3(1.1f));
        break;
    case 0b010000: // +x
        m_selectedFace = glm::rotate(glm::mat4(), glm::radians(90.f), glm::vec3(0, 1, 0));
        break;
    case 0b001000: // -z
        m_selectedFace = glm::rotate(glm::mat4(), glm::radians(180.f), glm::vec3(0, 1, 0));
        break;
    case 0b000100: // -x
        m_selectedFace = glm::rotate(glm::mat4(), glm::radians(-90.f), glm::vec3(0, 1, 0));
        break;
    case 0b000010: // +y
        m_selectedFace = glm::rotate(glm::mat4(), glm::radians(-90.f), glm::vec3(1, 0, 0));
        break;
    case 0b000001: // -y
        m_selectedFace = glm::rotate(glm::mat4(), glm::radians(90.f), glm::vec3(1, 0, 0));
        break;
    }

        m_selectedFace[3] = glm::vec4(point, 1.f);
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

    // highlighted face
    g->setColor(0, 0, 0, 1, 0);
    g->drawCube(m_selectedFace);
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
