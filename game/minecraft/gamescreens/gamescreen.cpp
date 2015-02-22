#include "gamescreen.h"
#include "camera.h"
#include "application.h"
#include "minecraftworld.h"
#include "mcchunkbuilder.h"
#include "player.h"

#include <QTime>

#define GLM_FORCE_RADIANS
#include <glm/gtx/transform.hpp>

#include <iostream>
using namespace std;

GameScreen::GameScreen(Application *parent)
    : Screen(parent)
{
    ActionCamera *cam = new ActionCamera();

    VoxelManager *vm = new VoxelManager(cam, m_parentApp->getShader(SPARSE), Point(7, 2, 7), Point(32, 32, 32),
                                        new MCChunkBuilder(QTime::currentTime().msec()));

    m_world = new MinecraftWorld(cam, vm);
    m_player = new Player(cam, glm::vec3(5.1f, 30.1f, 5.1f), m_world);

    m_world->addMovableEntity(m_player);

    this->setCamera(cam);

    m_parentApp->setUseCubeMap(true);
}

GameScreen::~GameScreen()
{
    delete m_world;
}

// update and render
void GameScreen::onTick(float secs)
{
    m_world->onTick(secs);
    m_player->setCameraPos();
}

void GameScreen::onRender(Graphics *g)
{
    int mode = m_player->getMode();
    glm::vec3 pos = m_player->getPosition();

    g->setGraphicsMode(SPARSE);
    g->setPlayer(pos, mode);
    g->setTint(1, 1, 1);

    if (mode == 0)
        g->resetParticles();
    else
    {
        if (mode == 1)
            g->setParticleForce(glm::vec3(0, -22, 0));
        else if (mode == 2)
            g->setParticleForce(glm::vec3(0, -45, 0));

        pos.y -= 1.f;
        g->drawParticles(pos);
    }


    m_world->onDraw(g);
}



// key events
void GameScreen::onKeyPressed(QKeyEvent *e)
{
    m_player->onKeyPressed(e);
}

void GameScreen::onKeyReleased(QKeyEvent *e)
{
    m_player->onKeyReleased(e);
}

void GameScreen::onMouseMoved(QMouseEvent *e, float deltaX, float deltaY)
{
    m_player->onMouseMoved(e, deltaX, deltaY);
}


// unused in menu
void GameScreen::onMousePressed(QMouseEvent *) {}
void GameScreen::onMouseReleased(QMouseEvent *) {}
void GameScreen::onMouseDragged(QMouseEvent *, float, float) {}
void GameScreen::onMouseWheel(QWheelEvent *) {}
