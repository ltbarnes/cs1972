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
    m_player = new Player(cam, glm::vec3(0,0,5), m_world);

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
    g->setColor(1.f, 1.f, 1.f, 1.f, 1.f);
    g->setWorldColor(1.f, 1.f, 1.f);
    g->setTexture("enter.jpg");

    m_world->onDraw(g);
}

// key events
void GameScreen::onKeyPressed(QKeyEvent *e)
{
    switch(e->key())
    {
    case Qt::Key_Return:
//        m_parentApp->addScreen(new GameScreen(m_parentApp));
        break;
    case Qt::Key_1:
        m_world->addBlock(-5, 0, 1, 1);
        break;
    case Qt::Key_2:
        m_world->addBlock(-3, 0, 1, 1);
        break;
    case Qt::Key_3:
        m_world->addBlock(-1, 0, 1, 1);
        break;
    case Qt::Key_4:
        m_world->addBlock(1, 0, 1, 1);
        break;
    case Qt::Key_5:
        m_world->addBlock(1, 0, 1, 1);
        break;
    default:
        break;
    }
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
