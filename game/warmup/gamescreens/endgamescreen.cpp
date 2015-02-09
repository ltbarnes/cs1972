#include "endgamescreen.h"
#include "camera.h"
#include "application.h"

#define GLM_FORCE_RADIANS
#include <glm/gtx/transform.hpp>

EndGameScreen::EndGameScreen(Application *parent, bool won)
    : Screen(parent)
{
    m_won = won;
}

EndGameScreen::~EndGameScreen()
{
}

// update and render
void EndGameScreen::onTick(float) {}

void EndGameScreen::onRender(Graphics *g)
{
    g->setColor(1.f, 1.f, 1.f, 1.f, 1.f);
    g->setWorldColor(1.f, 1.f, 1.f);
    if (m_won)
        g->setTexture("win.jpg");
    else
        g->setTexture("lose.jpg");
    g->drawQuad(glm::scale(glm::mat4(), glm::vec3(3, 3, 0)));
}

// key events
void EndGameScreen::onKeyPressed(QKeyEvent *e)
{
    switch (e->key())
    {
    case Qt::Key_Enter:
    case Qt::Key_Return:
        m_parentApp->popScreens(2);
        break;
    default:
        break;
    }
}


// unused in menu
void EndGameScreen::onMousePressed(QMouseEvent *) {}
void EndGameScreen::onMouseMoved(QMouseEvent *, float, float) {}
void EndGameScreen::onMouseReleased(QMouseEvent *) {}
void EndGameScreen::onMouseDragged(QMouseEvent *, float, float) {}
void EndGameScreen::onMouseWheel(QWheelEvent *) {}
void EndGameScreen::onKeyReleased(QKeyEvent *) {}
