#include "minecraftmenu.h"
#include "camera.h"
#include "application.h"
#include "gamescreen.h"

#include <cstdlib>

#define GLM_FORCE_RADIANS
#include <glm/gtx/transform.hpp>

#include <iostream>
using namespace std;


MinecraftMenu::MinecraftMenu(Application *parent)
    : Screen(parent)
{
    uint seed = 500;
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            srand(seed);
            srand(rand() * (j + 1));
            srand(rand() * (i + 1));
            cout << (rand() % 64) << ", ";
        }
        cout << endl;
    }

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
