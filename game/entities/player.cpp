#include "player.h"

#define EYE_HEIGHT 1.f

#include "printing.h"

Player::Player(ActionCamera *camera, glm::vec3 pos)
    : MovableEntity(pos)
{
    m_camera = camera;
    m_eye = m_pos + glm::vec3(0.f, EYE_HEIGHT, 0.f);
    m_camera->setCenter(m_eye);

    m_goalVel = glm::vec4(0.f);

    m_jump = false;
    m_crouch = false;
    m_walk = false;
    m_sprint = false;

    RenderShape *rs = new RenderShape();
    rs->type = CYLINDER;
    rs->color = glm::vec3(0, 1, 0);
    rs->shininess = 32.f;
    rs->trans = glm::scale(glm::mat4(), glm::vec3(1.f, 2.f, 1.f));
    rs->texture = "";
    rs->repeatU = 1.f;
    rs->repeatV = 1.f;

    addRenderShape(rs);
}


Player::~Player()
{
}


void Player::onTick(float secs)
{
    MovableEntity::onTick(secs);

    float eyeY = EYE_HEIGHT;
    float movementAmount = 5.f;

    if (m_walk)
        movementAmount = 2.f;
    if (m_sprint && m_goalVel.x > 0.5f)
        movementAmount = 8.f;
    if (m_crouch && m_pos.y <= 1.f)
    {
        movementAmount = 2.f;
        eyeY = 0.f;
    }

    // get camera look vector
    glm::vec4 look = m_camera->getLook();

    // set goal velocity
    glm::vec3 goal = glm::vec3(look.x, 0.f, look.z) * (m_goalVel.x + m_goalVel.y);
    goal += glm::vec3(-look.z, 0.f, look.x) * (m_goalVel.z + m_goalVel.w);

    // normalize if not zero vec
    float mag = glm::distance(goal, glm::vec3(0.f));
    if (mag > 0.f)
        goal *= movementAmount / mag;

    // calculate new velocity and apply force
    glm::vec3 vel = 10.0f * (goal - m_vel);
    vel.y = 0.f;

    applyForce(vel);


    // feet are on the ground
    if (m_pos.y <= 1.f)
    {
        // jump
        if (m_jump)
        {
//            m_pos.y = 1.001f;
//            m_currVel.y = 7.f;
//            m_crouch = false;
            applyImpulse(glm::vec3(0.f, 7.f, 0.f));
        }
        // crouch
        else if (m_crouch)
        {
            eyeY = 0.f;
            movementAmount = 0.02f;
        }
        // plant feet on ground
        else
        {
            m_pos.y = 1.f;
            m_vel.y = 0.f;
        }
    }
    else
    {
//        m_currVel.y -= 10.f * secs;
//        m_pos.y += m_currVel.y * secs;
    }

    m_eye = m_pos + glm::vec3(0, eyeY, 0);
    m_camera->setCenter(m_eye);
}


void Player::onDraw(Graphics *g)
{
    if (m_camera->getOffset() <= 0.001f)
        return;
    Entity::onDraw(g);
}


void Player::onKeyPressed(QKeyEvent *e)
{
    float offset;

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
    case Qt::Key_Minus:
    case Qt::Key_Underscore:
        offset = m_camera->getOffset();
        offset += 1.f;
        if (offset > 10.f)
            offset = 10.f;
        m_camera->setOffset(offset);
        break;
    case Qt::Key_Plus:
    case Qt::Key_Equal:
        offset = m_camera->getOffset();
        offset -= 1.f;
        if (offset < 0.f)
            offset = 0.f;
        m_camera->setOffset(offset);
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
    default:
        break;
    }
}

