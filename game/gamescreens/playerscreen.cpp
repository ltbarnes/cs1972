#include "playerscreen.h"

#define GLM_FORCE_RADIANS
#include <glm/gtx/transform.hpp>

#include <iostream>
using namespace std;

float eyeHeight = 2.f;

PlayerScreen::PlayerScreen()
{
    m_movementDir = 0b00000;
    glm::vec4 eye = glm::vec4(0.f, eyeHeight, 5.f, 0.f);
    m_camera->setEye(eye);

    m_jumpVelocity = 0;
}

PlayerScreen::~PlayerScreen()
{
}

void PlayerScreen::onTick(float secs)
{

    float movementAmount = 0.05f;

    if (0b01000 & m_movementDir)
        m_camera->moveForward(movementAmount);
    if (0b00100 & m_movementDir)
        m_camera->moveBack(movementAmount);
    if (0b00010 & m_movementDir)
        m_camera->moveLeft(movementAmount);
    if (0b00001 & m_movementDir)
        m_camera->moveRight(movementAmount);

    glm::vec4 eye = m_camera->getEye();

    // feet are on the ground
    if (eye.y <= eyeHeight)
    {
        // jump
        if (0b10000 & m_movementDir)
        {
            eye.y = eyeHeight + 0.001f;
            m_jumpVelocity = 7.f;
            m_camera->setEye(eye);
        }
        else // plant feet on ground
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

}

void PlayerScreen::onRender(Graphics *g)
{
    g->setColor(0.f, 1.f, 0.f, 1.f);
    g->setTexture("grass", 15.f, 15.f);
    glm::mat4 trans = glm::rotate(glm::mat4(), glm::radians(-90.f), glm::vec3(1.f, 0.f, 0.f));
    trans = glm::scale(trans, glm::vec3(50.f, 50.f, 0.f));
    g->drawQuad(trans);

    g->setTexture(NULL, 15.f, 15.f);
    g->setColor(0.f, 0.f, 1.f, 1.f);
    trans = glm::translate(glm::mat4(), glm::vec3(0, 2, 0));
    g->drawCube(trans);
}

void PlayerScreen::onMousePressed(QMouseEvent *)
{
}

void PlayerScreen::onMouseMoved(QMouseEvent *, float deltaX, float deltaY)
{
    m_camera->yaw(deltaX);
    m_camera->pitch(deltaY);
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
    glm::vec4 eye;

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
    default:
        break;
    }

}

