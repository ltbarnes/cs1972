#include "playerscreen.h"
#include "item.h"
#include "ground.h"
#include "application.h"

#include "printing.h"

float eyeHeight = 2.f;

bool up;
bool down;

PlayerScreen::PlayerScreen(Application *parent)
    : Screen(parent)
{
    ActionCamera *cam = new ActionCamera();

    m_world = new GameWorld();

    m_ufo = new UFO(cam, glm::vec3(0.f, 5.f, 5.f), m_world);
    m_player = new Player(cam, glm::vec3(0.f));

    m_world->addMovableEntity(m_player);
    m_world->addMovableEntity(m_ufo);

    float angle;
    for (int i = 0; i < 15; i++)
    {
        angle = 6.283f * i / 15.f;
        m_world->addMovableEntity(new Item(5.f * glm::vec3(glm::cos(angle), 0.f, glm::sin(angle)), m_player));
    }
    m_world->addStaticEntity(new Ground(glm::vec3(0, -1, 0)));

    this->setCamera(cam);
    m_parentApp->setUseCubeMap(true);

    m_inShip = false;
}

PlayerScreen::~PlayerScreen()
{
//    delete m_player; // world deletes player
    delete m_world;
}

void PlayerScreen::onTick(float secs)
{
     m_world->onTick(secs);

     if (m_inShip)
         m_ufo->setCameraPos();
     else
         m_player->setCameraPos();

    // will be an entity in world so this won't be necessary
//    m_player->onTick(secs);
}

void PlayerScreen::onRender(Graphics *g)
{
     m_world->onDraw(g);
}

void PlayerScreen::onMousePressed(QMouseEvent *)
{
}

void PlayerScreen::onMouseMoved(QMouseEvent *e, float deltaX, float deltaY)
{
    if (m_inShip)
        m_ufo->onMouseMoved(e, deltaX, deltaY);
    else
        m_player->onMouseMoved(e, deltaX, deltaY);
}

void PlayerScreen::onMouseReleased(QMouseEvent *)
{

}

void PlayerScreen::onMouseDragged(QMouseEvent *, float, float)
{
}

void PlayerScreen::onMouseWheel(QWheelEvent *)
{

}

void PlayerScreen::onKeyPressed(QKeyEvent *e)
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

void PlayerScreen::onKeyReleased(QKeyEvent *e)
{
    if (m_inShip)
        m_ufo->onKeyReleased(e);
    else
        m_player->onKeyReleased(e);
}

