#include "minecraftmenu.h"
#include "camera.h"
#include "application.h"
#include "gamescreen.h"

#define GLM_FORCE_RADIANS
#include <glm/gtx/transform.hpp>


MinecraftMenu::MinecraftMenu(Application *parent)
    : Screen(parent)
{
}

MinecraftMenu::~MinecraftMenu()
{

}

// update and render
void MinecraftMenu::onTick(float) {}

void MinecraftMenu::onRender(Graphics *g)
{
    g->setColor(1.f, 1.f, 1.f, 1.f, 1.f);
    g->setWorldColor(1.f, 1.f, 1.f);
    g->setTexture("enter.jpg");
    g->drawQuad(glm::scale(glm::mat4(), glm::vec3(2, 1.5f, 0)));
}

// key events
void MinecraftMenu::onKeyPressed(QKeyEvent *e)
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
void MinecraftMenu::onMousePressed(QMouseEvent *) {}
void MinecraftMenu::onMouseMoved(QMouseEvent *, float, float) {}
void MinecraftMenu::onMouseReleased(QMouseEvent *) {}
void MinecraftMenu::onMouseDragged(QMouseEvent *, float, float) {}
void MinecraftMenu::onMouseWheel(QWheelEvent *) {}
void MinecraftMenu::onKeyReleased(QKeyEvent *) {}
