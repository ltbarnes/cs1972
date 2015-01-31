#include "gameworld.h"

#define GLM_FORCE_RADIANS
#include <glm/gtx/transform.hpp>

GameWorld::GameWorld()
{
    m_light = new Light();
    m_light->id = 0;
    m_light->color = glm::vec3(1.f, 0.8f, 0.6f);
    m_light->pos = glm::vec3(5.f, -1.f, 1.f); // actually dir
}


GameWorld::~GameWorld()
{
    delete m_light;
}


void GameWorld::onTick(float secs)
{
    World::onTick(secs);
    // gravity
    foreach(MovableEntity *e, m_movableEntities) // f = ma (a = g)
        e->applyForce(glm::vec3(0.f, -10.f, 0.f) * e->getMass());

}


void GameWorld::onDraw(Graphics *g)
{
    g->useCubeMap(true);
    g->setWorldColor(0.3f, 0.35f, 0.4f);

    g->addLight(*m_light);

    g->setColor(0.75f, 0.8f, 0.9f, 128.f);
    g->setTexture("snow.jpg", 15.f, 15.f);

    // draw ground
    glm::mat4 trans = glm::translate(glm::mat4(), glm::vec3(0.f, -.5f, 0.f));
    trans = glm::scale(trans, glm::vec3(50.f, 1.f, 50.f));
    g->drawCyl(trans);

    // draw cube
    g->setTexture("");
    g->setColor(.5f, .7f, 1.f, 64.f);
    trans = glm::translate(glm::mat4(), glm::vec3(0, 2, 0));
    g->drawCube(trans);

    // draw cylinder
    g->setColor(1.f, .5f, .7f, 128.f);
    trans = glm::translate(glm::mat4(), glm::vec3(2, 2, -1));
    g->drawCyl(trans);

    // draw sphere
    g->setColor(.7f, 1.f, .5f, 256.f);
    trans = glm::translate(glm::mat4(), glm::vec3(2, 1, 1));
    g->drawSphere(trans);

    World::onDraw(g);

}

