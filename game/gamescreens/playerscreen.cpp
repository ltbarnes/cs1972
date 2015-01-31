#include "playerscreen.h"

//#include "printing.h"

float eyeHeight = 2.f;

bool up;
bool down;

PlayerScreen::PlayerScreen()
{
    ActionCamera *cam = new ActionCamera();
    m_player = new Player(cam, glm::vec3(0.f, 1.f, 5.f));
    m_world = new GameWorld();
    m_world->addMovableEntity(m_player);

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

void PlayerScreen::onMouseMoved(QMouseEvent *, float deltaX, float deltaY)
{
    m_camera->yaw(deltaX / 10.f);
    m_camera->pitch(deltaY / 10.f);
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
    m_player->onKeyPressed(e);
}

void PlayerScreen::onKeyReleased(QKeyEvent *e)
{
    m_player->onKeyReleased(e);
}

