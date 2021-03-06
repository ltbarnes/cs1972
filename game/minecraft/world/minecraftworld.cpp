#include "minecraftworld.h"
#include "point3d.h"
#include <QSet>

#include <iostream>
using namespace std;

MinecraftWorld::MinecraftWorld(Camera *cam, VoxelManager *vm, bool create)
{
    m_vm = vm;
    m_vm->setAtlas("terrain.png");
    addManager(m_vm);

    m_camera = cam;
    m_selectedTrans = glm::mat4();
    m_selectedBlock = Point();
    m_selectedFace = Point();

    m_createMode = create;
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

    if (m_createMode)
    {
        float t;
        int face = 0;
        glm::vec3 point = m_vm->castRay(glm::vec3(m_camera->getEye()), glm::vec3(m_camera->getLook()), t, face);
        m_selectedBlock = Point((int)point.x, (int)point.y, (int)point.z);

        switch(face)
        {
        case 0b100000: // +z
            m_selectedTrans = glm::mat4();
            point.z += .503f;
            m_selectedFace = Point(0, 0, 1);
            break;
        case 0b010000: // -z
            m_selectedTrans = glm::rotate(glm::mat4(), glm::radians(180.f), glm::vec3(0, 1, 0));
            point.z -= .503f;
            m_selectedFace = Point(0, 0,-1);
            break;
        case 0b001000: // +x
            m_selectedTrans = glm::rotate(glm::mat4(), glm::radians(90.f), glm::vec3(0, 1, 0));
            point.x += .503f;
            m_selectedFace = Point(1, 0, 0);
            break;
        case 0b000100: // -x
            m_selectedTrans = glm::rotate(glm::mat4(), glm::radians(-90.f), glm::vec3(0, 1, 0));
            point.x -= .503f;
            m_selectedFace = Point(-1, 0, 0);
            break;
        case 0b000010: // +y
            m_selectedTrans = glm::rotate(glm::mat4(), glm::radians(-90.f), glm::vec3(1, 0, 0));
            point.y += .503f;
            m_selectedFace = Point(0, 1, 0);
            break;
        case 0b000001: // -y
            m_selectedTrans = glm::rotate(glm::mat4(), glm::radians(90.f), glm::vec3(1, 0, 0));
            point.y -= .503f;
            m_selectedFace = Point(0,-1, 0);
            break;
        }

        m_selectedTrans[3] = glm::vec4(point, 1.f);
    }
}

void MinecraftWorld::onDraw(Graphics *g)
{
    g->setGraphicsMode(DEFAULT);

    Light light;
    light.color = glm::vec3(1.f, 0.f, 0.f);
    light.posDir = glm::vec3(0, -1, 0);
    light.id = 1;
    light.type = 1;

    g->setWorldColor(.1f, .4f, .1f);
    g->addLight(light);

    World::onDraw(g);

    if (m_createMode)
    {
        // highlighted face
        g->setColor(0, 0, 0, 1, 0);
        g->drawQuad(m_selectedTrans, GL_LINES);
    }
}

glm::vec4 MinecraftWorld::getCoords(int index, Point dim)
{
    return glm::vec4(((index / dim.y) % dim.x),
                     (index % dim.y),
                     (index / (dim.x*dim.y)),
                     0.f);
}


void MinecraftWorld::addBlock()
{
    Point p = m_selectedBlock + m_selectedFace;
    m_vm->addBlock(p);
}


void MinecraftWorld::removeBlock()
{
    m_vm->removeBlock(m_selectedBlock);
}
