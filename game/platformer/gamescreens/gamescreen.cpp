#include "gamescreen.h"
#include "application.h"

GameScreen::GameScreen(Application *parent, int level)
    : Screen(parent)
{
    m_parentApp->setUseCubeMap(true);

    switch (level)
    {
    case 2:
        m_level->read("/levels/level_hard.obj");
        break;
    case 3:
        m_level->read("/levels/level_island.obj");
        break;
    default: // 1
        m_level->read("/levels/level_easy.obj");
        break;
    }
}

GameScreen::~GameScreen()
{
}

// update and render
void GameScreen::onTick(float) {}

void GameScreen::onRender(Graphics *)
{

}

void GameScreen::onMouseMoved(QMouseEvent *, float, float)
{
}

void GameScreen::onMousePressed(QMouseEvent *)
{
}


// unused in game
void GameScreen::onMouseReleased(QMouseEvent *) {}
void GameScreen::onMouseDragged(QMouseEvent *, float, float) {}
void GameScreen::onMouseWheel(QWheelEvent *) {}
void GameScreen::onKeyPressed(QKeyEvent *) {}
void GameScreen::onKeyReleased(QKeyEvent *) {}
