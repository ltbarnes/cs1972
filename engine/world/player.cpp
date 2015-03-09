#include "player.h"

Player::Player(ActionCamera *cam, glm::vec3 pos)
    : MovableEntity(pos)
{
    m_camera = cam;
    m_wsad = 0;
    m_up = false;
    m_down = false;
}


Player::~Player()
{
}


void Player::onTick(float)
{
    glm::vec3 force = glm::vec3();
    if (m_wsad & 0b1000)
        force.z += 1;
    if (m_wsad & 0b0100)
        force.z -= 1;
    if (m_wsad & 0b0010)
        force.x -= 1;
    if (m_wsad & 0b0001)
        force.x += 1;
    if (m_up)
        force.y += 1;
    if (m_down)
        force.y -= 1;

    glm::vec4 look = m_camera->getLook();

    glm::vec3 thrust = glm::normalize(glm::vec3(look.x, 0.f, look.z)) * force.z;
    thrust += glm::normalize(glm::vec3(-look.z, 0.f, look.x)) * force.x;
    thrust.y = force.y;

    setPosition(getPosition() + thrust * .1f);
}


void Player::setCameraPos()
{
    m_camera->setCenter(getPosition() + glm::vec3(0, 2, 0));
}


void Player::onMouseMoved(QMouseEvent *, float deltaX, float deltaY)
{
    m_camera->yaw(deltaX / 10.f);
    m_camera->pitch(deltaY / 10.f);
}

void Player::onKeyPressed(QKeyEvent *e)
{
    switch (e->key())
    {
    case Qt::Key_W:
        m_wsad |= 0b1000;
        break;
    case Qt::Key_S:
        m_wsad |= 0b0100;
        break;
    case Qt::Key_A:
        m_wsad |= 0b0010;
        break;
    case Qt::Key_D:
        m_wsad |= 0b0001;
        break;
    case Qt::Key_E:
        m_up = true;
        break;
    case Qt::Key_Q:
        m_down = true;
        break;
    default:
        break;
    }
}

void Player::onKeyReleased(QKeyEvent *e)
{
    switch (e->key())
    {
    case Qt::Key_W:
        m_wsad &= 0b0111;
        break;
    case Qt::Key_S:
        m_wsad &= 0b1011;
        break;
    case Qt::Key_A:
        m_wsad &= 0b1101;
        break;
    case Qt::Key_D:
        m_wsad &= 0b1110;
        break;
    case Qt::Key_E:
        m_up = false;
        break;
    case Qt::Key_Q:
        m_down = false;
        break;
//    case Qt::Key_Space:
//        m_jump = false;
//        m_up = false;
//        break;
//    case Qt::Key_Shift:
//        m_jetMode = false;
//        if (m_up)
//        {
//            m_jump = true;
//            m_up = false;
//        }
//        break;
    default:
        break;
    }
}



