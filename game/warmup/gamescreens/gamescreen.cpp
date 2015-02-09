#include "gamescreen.h"
#include "item.h"
#include "ground.h"
#include "application.h"
#include "endgamescreen.h"

float eyeHeight = 2.f;

bool up;
bool down;

GameScreen::GameScreen(Application *parent)
    : Screen(parent)
{
    ActionCamera *cam = new ActionCamera();

    m_world = new GameWorld();

    m_ufo = new UFO(cam, glm::vec3(25.f, 25.f, -25.f), m_world);
    m_player = new Player(cam, glm::vec3(0.f));

    m_world->addMovableEntity(m_player);
    m_world->addMovableEntity(m_ufo);

    float angle;
    for (int i = 0; i < 10; i++)
    {
        angle = 6.283f * i / 14.f;
        m_world->addMovableEntity(new Item(5.f * glm::vec3(glm::cos(angle), 0.f, glm::sin(angle)), m_player, m_parentApp));
    }
    m_world->addStaticEntity(new Ground(glm::vec3(0, -1, 0)));

    this->setCamera(cam);
    m_parentApp->setUseCubeMap(true);

    m_inShip = false;
}

GameScreen::~GameScreen()
{
    if (m_inShip)
        delete m_player; // world deletes player otherwise
    delete m_world;
}

void GameScreen::onTick(float secs)
{
    if (m_ufo->getNumRemoved() >= 10)
        m_parentApp->addScreen(new EndGameScreen(m_parentApp, true));
     m_world->onTick(secs);

     if (m_inShip)
     {
         m_ufo->setCameraPos();
         m_player->setPosition(m_ufo->getPosition());
     }
     else
         m_player->setCameraPos();

    // will be an entity in world so this won't be necessary
//    m_player->onTick(secs);
}

void GameScreen::onRender(Graphics *g)
{
     m_world->onDraw(g);
}

void GameScreen::onMousePressed(QMouseEvent *)
{
}

void GameScreen::onMouseMoved(QMouseEvent *e, float deltaX, float deltaY)
{
    if (m_inShip)
        m_ufo->onMouseMoved(e, deltaX, deltaY);
    else
        m_player->onMouseMoved(e, deltaX, deltaY);
}

void GameScreen::onMouseReleased(QMouseEvent *)
{

}

void GameScreen::onMouseDragged(QMouseEvent *, float, float)
{
}

void GameScreen::onMouseWheel(QWheelEvent *)
{

}

void GameScreen::onKeyPressed(QKeyEvent *e)
{
    if (e->key() == Qt::Key_F)
    {
        if (m_inShip)
        {
            m_inShip = false;
            m_player->reset();
            m_player->setPosition(m_ufo->getPosition() + glm::vec3(0.f, 5.f, 0.f));
            m_player->setVelocity(m_ufo->getVelocity());
            m_world->addMovableEntity(m_player);
        }
        else if (glm::distance(m_player->getPosition(), m_ufo->getPosition()) < 5.f)
        {
            m_inShip = true;
            m_ufo->reset();
            m_world->removeMovableEntity(m_player, false);
        }
    }
    if (m_inShip)
        m_ufo->onKeyPressed(e);
    else
        m_player->onKeyPressed(e);
}

void GameScreen::onKeyReleased(QKeyEvent *e)
{
    if (m_inShip)
        m_ufo->onKeyReleased(e);
    else
        m_player->onKeyReleased(e);
}

