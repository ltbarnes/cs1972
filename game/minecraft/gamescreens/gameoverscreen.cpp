#include "gameoverscreen.h"
#include "camera.h"
#include "application.h"


#include <cstdlib>

#define GLM_FORCE_RADIANS
#include <glm/gtx/transform.hpp>


GameOverScreen::GameOverScreen(Application *parent, int gameStatus)
    : Screen(parent)
{
    m_gameStatus = gameStatus;
    m_parentApp->setUseCubeMap(true);
}

GameOverScreen::~GameOverScreen()
{

}

// update and render
void GameOverScreen::onTick(float) {}

void GameOverScreen::onRender(Graphics *g)
{
    g->setGraphicsMode(DEFAULT);
    g->setColor(1.f, 1.f, 1.f, 1.f, 1.f);
    g->setWorldColor(1.f, 1.f, 1.f);

    if (m_gameStatus == 1)
        g->setTexture("debris.png");
    else if (m_gameStatus == 2)
        g->setTexture("ghost.png");
    else
        g->setTexture("win.jpg");

    g->drawQuad(glm::scale(glm::mat4(), glm::vec3(2.5f, 1.5f, 0)));
}

// key events
void GameOverScreen::onKeyPressed(QKeyEvent *e)
{
    switch (e->key())
    {
    case Qt::Key_W:
    case Qt::Key_A:
    case Qt::Key_S:
    case Qt::Key_D:
    case Qt::Key_E:
    case Qt::Key_Shift:
    case Qt::Key_Space:
        break;
    default:
        m_parentApp->popScreens(2);
        break;
    }
}


// unused in menu
void GameOverScreen::onMousePressed(QMouseEvent *) {}
void GameOverScreen::onMouseMoved(QMouseEvent *, float, float) {}
void GameOverScreen::onMouseReleased(QMouseEvent *) {}
void GameOverScreen::onMouseDragged(QMouseEvent *, float, float) {}
void GameOverScreen::onMouseWheel(QWheelEvent *) {}
void GameOverScreen::onKeyReleased(QKeyEvent *) {}
