#include "gamescreen.h"
#include "camera.h"
#include "application.h"
#include "minecraftworld.h"
#include "player.h"

#define GLM_FORCE_RADIANS
#include <glm/gtx/transform.hpp>

#include <iostream>
using namespace std;

GameScreen::GameScreen(Application *parent)
    : Screen(parent)
{
    ActionCamera *cam = new ActionCamera();
    m_world = new MinecraftWorld();
    m_player = new Player(cam, glm::vec3(0,5,0), m_world);

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
