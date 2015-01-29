#include "player.h"

#define EYE_HEIGHT 2.f

Player::Player(ActionCamera *camera)
{
    m_camera = camera;
    m_goalVel = glm::vec4(0.f);
    m_jump = false;
}


Player::~Player()
{
}


void Player::onTick(float secs)
{
//    float movementAmount = 0.5f;
    float movementAmount = 0.05f;
    if (m_walk)
        movementAmount = 0.02f;
    if (m_sprint && m_goalVel.x > 0.5f)
        movementAmount = 0.08f;

    float y = m_camera->getEye().y;

    // feet are on the ground
    if (y <= EYE_HEIGHT)
    {
        // jump
        if (m_jump)
        {
            y = EYE_HEIGHT + 0.001f;
            m_jumpVelocity = 7.f;
            m_crouch = false;
        }
        // crouch
        else if (m_crouch)
        {
            y = EYE_HEIGHT / 2.f;
            movementAmount = 0.02f;
        }
        // plant feet on ground
        else
        {
        y = EYE_HEIGHT;
        m_jumpVelocity = 0.f;
        }
    }
    else
    {
        m_jumpVelocity -= 10.f * secs;
        y += m_jumpVelocity * secs;
    }

    m_currVel.z = secs * 0.1f * ((m_goalVel.x + m_goalVel.y) - m_currVel.z) * movementAmount;
    m_currVel.x = secs * 0.1f * ((m_goalVel.z + m_goalVel.w) - m_currVel.x) * movementAmount;



    m_camera->moveRelativeToLook(glm::vec3(0, y, 0));
}


void Player::onDraw(Graphics *)
{
    if (m_camera->getThirdPersonDistance() <= 0.0000001f)
        return;
}


void Player::onKeyPressed(QKeyEvent *e)
{
    switch (e->key())
    {
    case Qt::Key_W:
        m_goalVel.x = 1.f;
        break;
    case Qt::Key_S:
        m_goalVel.y = -1.f;
        break;
    case Qt::Key_A:
        m_goalVel.z = -1.f;
        break;
    case Qt::Key_D:
        m_goalVel.w = 1.f;
        break;
    case Qt::Key_Space:
        m_jump = true;
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


void Player::onKeyReleased(QKeyEvent *e)
{
    switch (e->key())
    {
    case Qt::Key_W:
        m_goalVel.x = 0.f;
        break;
    case Qt::Key_S:
        m_goalVel.y = 0.f;
        break;
    case Qt::Key_A:
        m_goalVel.z = 0.f;
        break;
    case Qt::Key_D:
        m_goalVel.w = 0.f;
        break;
    case Qt::Key_Space:
        m_jump = false;
        break;
    case Qt::Key_Shift:
        m_sprint = false;
        break;
    case Qt::Key_CapsLock:
        m_walk = false;
        break;
    case Qt::Key_Q:
//        down = false;
        break;
    case Qt::Key_E:
//        up = false;
        break;
    default:
        break;
    }
}

