#include "menuscreen.h"
#include "camera.h"
#include "application.h"
#include "gamescreen.h"

#define GLM_FORCE_RADIANS
#include <glm/gtx/transform.hpp>

#include <iostream>

using namespace std;

MenuScreen::MenuScreen(Application *parent)
    : Screen(parent)
{
}

MenuScreen::~MenuScreen()
{

}

// update and render
void MenuScreen::onTick(float) {}

void MenuScreen::onRender(Graphics *g)
{
    g->setColor(1.f, 1.f, 1.f, 1.f, 1.f);
    g->setWorldColor(1.f, 1.f, 1.f);
    g->setTexture("enter.jpg");
    g->drawQuad(glm::scale(glm::mat4(), glm::vec3(2, 1.5f, 0)));
}

// key events
void MenuScreen::onKeyPressed(QKeyEvent *e)
{
    switch(e->key())
    {
    case Qt::Key_Return:
        m_parentApp->addScreen(new GameScreen(m_parentApp));
        break;
    default:
        break;
    }
}


// unused in menu
void MenuScreen::onMousePressed(QMouseEvent *) {}
void MenuScreen::onMouseMoved(QMouseEvent *, float, float) {}
void MenuScreen::onMouseReleased(QMouseEvent *) {}
void MenuScreen::onMouseDragged(QMouseEvent *, float, float) {}
void MenuScreen::onMouseWheel(QWheelEvent *) {}
void MenuScreen::onKeyReleased(QKeyEvent *) {}
