#include "playerscreen.h"
#include <QTime>

#define GLM_FORCE_RADIANS
#include <glm/gtx/transform.hpp>

//#include "printing.h"

float eyeHeight = 2.f;

bool up;
bool down;

PlayerScreen::PlayerScreen()
{
//    Camera *cam = new ActionCamera();
//    m_player = new Player(cam);
//    m_world = new World();
//    m_world->addEntity(m_player);

//    this->setCamera(m_player->getCamera());

    // each bit represents a movement:
    // jump, forward, back, left, right
    m_movementDir = 0b00000;
    glm::vec4 eye = glm::vec4(0.f, eyeHeight, 5.f, 0.f);
    m_camera->setEye(eye);

    m_jumpVelocity = 0;
    m_crouch = false;
    m_walk = false;
    m_sprint = false;

    up = false;
    down = false;
}

PlayerScreen::~PlayerScreen()
{
//    delete m_player;
//    delete m_world;
}

void PlayerScreen::onTick(float secs)
{

//    float movementAmount = 0.5f;
    float movementAmount = 0.05f;
    if (m_walk)
        movementAmount = 0.02f;
    if (m_sprint && 0b01000 & m_movementDir)
        movementAmount = 0.08f;

    glm::vec4 eye = m_camera->getEye();
    if (up)
        eye.y += 0.5f;
    if (down)
        eye.y -= 0.5f;
    // feet are on the ground
    if (eye.y <= eyeHeight)
    {
        // jump
        if (0b10000 & m_movementDir)
        {
            eye.y = eyeHeight + 0.001f;
            m_jumpVelocity = 7.f;
            m_crouch = false;
        }
        // crouch
        else if (m_crouch)
        {
            eye.y = eyeHeight / 2.f;
            movementAmount = 0.02f;
        }
        // plant feet on ground
        else
        {
        eye.y = eyeHeight;
        m_jumpVelocity = 0.f;
        }
    }
    else
    {
        m_jumpVelocity -= 10.f * secs;
        eye.y += m_jumpVelocity * secs;
    }
    m_camera->setEye(eye);

    if (0b01000 & m_movementDir)
        m_camera->moveForward(movementAmount);
    if (0b00100 & m_movementDir)
        m_camera->moveBack(movementAmount);
    if (0b00010 & m_movementDir)
        m_camera->moveLeft(movementAmount);
    if (0b00001 & m_movementDir)
        m_camera->moveRight(movementAmount);

}

void PlayerScreen::onRender(Graphics *g)
{
    g->useCubeMap(true);
    g->setWorldColor(0.3f, 0.35f, 0.4f);

    Light *light = new Light();
    light->id = 0;
    light->color = glm::vec3(1.f, 0.8f, 0.6f);
    light->pos = glm::vec3(5.f, -1.f, 1.f); // actually dir

    g->addLight(*light);


    g->setColor(0.75f, 0.8f, 0.9f, 128.f);
//    g->setTexture("snow.jpg", 15.f, 15.f);
    g->setTexture("grass.png", 50.f, 50.f);

    glm::mat4 trans = glm::rotate(glm::mat4(), glm::radians(-90.f), glm::vec3(1.f, 0.f, 0.f));
    trans = glm::scale(trans, glm::vec3(50.f, 50.f, 1.f));
    g->drawQuad(trans);


    g->setTexture(NULL);
    g->setColor(.5f, .7f, 1.f, 64.f);
    trans = glm::translate(glm::mat4(), glm::vec3(0, 2, 0));
    g->drawCube(trans);

    delete light;
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
    switch (e->key())
    {
    case Qt::Key_W:
        m_movementDir |= 0b01000;
        break;
    case Qt::Key_S:
        m_movementDir |= 0b00100;
        break;
    case Qt::Key_A:
        m_movementDir |= 0b00010;
        break;
    case Qt::Key_D:
        m_movementDir |= 0b00001;
        break;
    case Qt::Key_Space:
        m_movementDir |= 0b10000;
        break;
    case Qt::Key_C:
        m_crouch = !m_crouch;
        break;
    case Qt::Key_Shift:
        m_sprint = true;
        break;
    case Qt::Key_CapsLock:
        m_walk = true;
        break;
    case Qt::Key_Q:
//        down = true;
        break;
    case Qt::Key_E:
//        up = true;
        break;
    default:
        break;
    }
}

void PlayerScreen::onKeyReleased(QKeyEvent *e)
{
    switch (e->key())
    {
    case Qt::Key_W:
        m_movementDir &= 0b10111;
        break;
    case Qt::Key_S:
        m_movementDir &= 0b11011;
        break;
    case Qt::Key_A:
        m_movementDir &= 0b11101;
        break;
    case Qt::Key_D:
        m_movementDir &= 0b11110;
        break;
    case Qt::Key_Space:
        m_movementDir &= 0b01111;
        break;
    case Qt::Key_Shift:
        m_sprint = false;
        break;
    case Qt::Key_CapsLock:
        m_walk = false;
        break;
    case Qt::Key_Q:
        down = false;
        break;
    case Qt::Key_E:
        up = false;
        break;
    default:
        break;
    }

}

