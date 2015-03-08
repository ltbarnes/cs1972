#include "platformermenu.h"
#include "application.h"
#include "gamescreen.h"

#define GLM_FORCE_RADIANS
#include <glm/gtx/transform.hpp>

PlatformerMenu::PlatformerMenu(Application *parent)
    : Screen(parent)
{
    m_buttonEasy = new Button();
    m_buttonEasy->setCenter(0.f, 0.f);
    m_buttonEasy->setSize(.4, .5);

    m_buttonHard = new Button();
    m_buttonHard->setCenter(-.6f, 0.f);
    m_buttonHard->setSize(.4, .5);

    m_buttonIsland = new Button();
    m_buttonIsland->setCenter(.6f, 0.f);
    m_buttonIsland->setSize(.4, .5);

    m_cursor = glm::scale(glm::mat4(), glm::vec3(.05, .05, .05));
    m_cursor[3][2] = -.999f;

    m_level = 0;
}

PlatformerMenu::~PlatformerMenu()
{
    delete m_buttonEasy;
    delete m_buttonHard;
    delete m_buttonIsland;
}

// update and render
void PlatformerMenu::onTick(float) {}

void PlatformerMenu::onRender(Graphics *g)
{
    m_parentApp->setUseCubeMap(false);

    g->setGraphicsMode(DRAW2D);
    m_buttonEasy->onDraw(g);
    m_buttonHard->onDraw(g);
    m_buttonIsland->onDraw(g);

    g->setColor(1, 0, 0, 1, 0);
    g->drawSphere(m_cursor);

}

void PlatformerMenu::onMouseMoved(QMouseEvent *, float deltaX, float deltaY)
{
    float newX = m_cursor[3][0] + deltaX * 0.005f;
    if (newX < -1.f)
        newX = -1.f;
    else if (newX > 1.f)
        newX = 1.f;
    m_cursor[3][0] = newX;

    float newY = m_cursor[3][1] - deltaY * 0.005f;
    if (newY < -1.f)
        newY = -1.f;
    else if (newY > 1.f)
        newY = 1.f;
    m_cursor[3][1] = newY;

    if (m_buttonEasy->contains(newX, newY))
        m_level = 1;
    else if (m_buttonHard->contains(newX, newY))
        m_level = 2;
    else if (m_buttonIsland->contains(newX, newY))
        m_level = 3;
    else
        m_level = 0;
}

void PlatformerMenu::onMousePressed(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton && m_level > 0)
        m_parentApp->addScreen(new GameScreen(m_parentApp, m_level));
}

void PlatformerMenu::onResize(int w, int h)
{
    Screen::onResize(w, h);

    float aspect = w * 1.f / h;
    glm::vec4 pos = m_cursor[3];
    m_cursor = glm::scale(glm::mat4(), glm::vec3(.03f / aspect, .03f, 1));
    m_cursor[3] = pos;
}


// unused in menu
void PlatformerMenu::onMouseReleased(QMouseEvent *) {}
void PlatformerMenu::onMouseDragged(QMouseEvent *, float, float) {}
void PlatformerMenu::onMouseWheel(QWheelEvent *) {}
void PlatformerMenu::onKeyPressed(QKeyEvent *) {}
void PlatformerMenu::onKeyReleased(QKeyEvent *) {}
