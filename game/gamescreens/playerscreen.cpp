#include "playerscreen.h"
#include "item.h"
#include "ground.h"

//#include "printing.h"

float eyeHeight = 2.f;

bool up;
bool down;

PlayerScreen::PlayerScreen()
{
    ActionCamera *cam = new ActionCamera();

    m_player = new Player(cam, glm::vec3(0.f, 1.f, 5.f));
    m_ufo = new UFO(cam, glm::vec3(0.f, 5.f, 5.f));
    m_world = new GameWorld();

    m_world->addMovableEntity(m_player);
    m_world->addMovableEntity(m_ufo);
    m_world->addMovableEntity(new Item(glm::vec3(0.f)));
    m_world->addMovableEntity(new Item(glm::vec3(0.f, 2.f, 0.f)));

    m_world->addStaticEntity(new Ground(glm::vec3(0, -1, 0)));

    this->setCamera(cam);
}

PlayerScreen::~PlayerScreen()
{
//    delete m_player; // world deletes player
    delete m_world;
}

void PlayerScreen::onTick(float secs)
{
     m_world->onTick(secs);

     m_player->setCameraPos();
//     m_ufo->setCameraPos();

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
//    m_ufo->onMouseMoved(e, deltaX, deltaY);
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
//    m_ufo->onKeyPressed(e);
    m_player->onKeyPressed(e);
}

void PlayerScreen::onKeyReleased(QKeyEvent *e)
{
//    m_ufo->onKeyReleased(e);
    m_player->onKeyReleased(e);
}

