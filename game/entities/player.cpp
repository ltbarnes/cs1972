#include "player.h"
#include "collisioncylinder.h"

#define EYE_HEIGHT 1.f

#include "printing.h"

Player::Player(ActionCamera *camera, glm::vec3 pos)
    : MovableEntity(pos)
{
    m_camera = camera;
    m_eyeHeight = EYE_HEIGHT;
    m_camera->setCenter(getPosition() + glm::vec3(0.f, m_eyeHeight, 0.f));

    m_offset = 0.f;
    reset();

    CollisionShape *cs = new CollisionCylinder(glm::vec3(), glm::vec3(1.f, 2.f, 1.f), "player");
    addCollisionShape(cs);

    RenderShape *rs = new RenderShape();
    rs->type = CYLINDER;
    rs->color = glm::vec3(0, 1, 0);
    rs->shininess = 32.f;
    rs->transparency = 1.f;
    rs->trans = glm::scale(glm::mat4(), glm::vec3(1.f, 2.f, 1.f));
    rs->texture = "";
    rs->repeatU = 1.f;
    rs->repeatV = 1.f;

    addRenderShape(rs);
}


Player::~Player()
{
}


void Player::reset()
{
    m_goalVel = glm::vec4(0.f);

    m_canJump = false;
    m_jump = false;
    m_crouch = false;
    m_walk = false;
    m_sprint = false;
    m_camera->setOffset(m_offset);
}


void Player::onTick(float secs)
{
    if (m_jump && m_canJump)
    {
        applyImpulse(glm::vec3(0.f, 7.f, 0.f));
    }
    m_canJump = false;

    MovableEntity::onTick(secs);

    m_eyeHeight = EYE_HEIGHT;
    float movementAmount = 5.f;

    if (m_walk)
        movementAmount = 2.f;
    if (m_sprint && m_goalVel.x > 0.5f)
        movementAmount = 8.f;
    if (m_crouch)
    {
        movementAmount = 2.f;
        m_eyeHeight = 0.f;
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
}


void Player::handleCollision(Collision *col)
{
    MovableEntity::handleCollision(col);
    if (col->mtv.y > 0.f && col->c2->getID() != "beam")
        m_canJump = true;
}


void Player::setCameraPos()
{
    m_camera->setCenter(getPosition() + glm::vec3(0.f, m_eyeHeight, 0.f));
}

void Player::onDrawOpaque(Graphics *g)
{
    if (m_camera->getOffset() <= 0.001f)
        return;
    Entity::onDrawOpaque(g);
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
        m_offset += 1.f;
        if (m_offset > 10.f)
            m_offset = 10.f;
        m_camera->setOffset(m_offset);
        break;
    case Qt::Key_Plus:
    case Qt::Key_Equal:
        m_offset -= 1.f;
        if (m_offset < 0.f)
            m_offset = 0.f;
        m_camera->setOffset(m_offset);
        break;
    case Qt::Key_0:
        m_offset = 0.f;
        m_camera->setOffset(m_offset);
        break;
    case Qt::Key_9:
        m_offset = 25.f;
        m_camera->setOffset(m_offset);
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

