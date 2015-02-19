#include "player.h"
#include "collisioncylinder.h"

#include <iostream>
using namespace std;
#include <glm/ext.hpp>

Player::Player(ActionCamera *camera, glm::vec3 pos, World *world)
    : MovableEntity(pos)
{
    m_camera = camera;
    m_world = world;

    m_wsad = 0b0000;
    m_up = false;
    m_down = false;

    m_offset = 0.f;
    setMass(1.f);

    m_forceAmt = 8.f;
    m_jump = false;
    m_canJump = false;

    CollisionShape *cs = new CollisionCylinder(glm::vec3(), glm::vec3(.99f, 1.98f, .99f), "player");
    addCollisionShape(cs);

    RenderShape *rs = new RenderShape();
    rs->type = CYLINDER;
    rs->color = glm::vec3(0, 1, 0);
    rs->shininess = 32.f;
    rs->transparency = 1.f;
    rs->trans = glm::scale(glm::mat4(), glm::vec3(.99f, 1.98f, .99f));
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

    glm::vec3 force = glm::vec3(0.f);
    if (m_wsad & 0b1000)
        force.z += 1;
    if (m_wsad & 0b0100)
        force.z -= 1;
    if (m_wsad & 0b0010)
        force.x -= 1;
    if (m_wsad & 0b0001)
        force.x += 1;
    if (m_up)
        applyForce(glm::vec3(0, (m_forceAmt * 1.5f + 13.f) * getMass(), 0));
//        force.y += m_forceAmt;
//    if (m_down)
//        force.y -= m_forceAmt;

    glm::vec4 look = m_camera->getLook();

    glm::vec3 thrust = glm::normalize(glm::vec3(look.x, 0.f, look.z)) * force.z;
    thrust += glm::normalize(glm::vec3(-look.z, 0.f, look.x)) * force.x;
    if (glm::length2(thrust) > 0.00001)
        thrust = glm::normalize(thrust) * m_forceAmt;
    thrust.y = force.y;

    glm::vec3 vel = thrust - m_vel;
    if (m_jump && m_canJump)
        vel.y = 9.f;
    else
        vel.y = 0.f;

    if (m_canJump)
        applyImpulse(vel);
    else
        applyForce(vel);

    m_canJump = false;
}


void Player::onDrawOpaque(Graphics *g)
{
    Entity::onDrawOpaque(g);
}

void Player::onDrawTransparent(Graphics *)
{
}


void Player::setCameraPos()
{
    m_camera->setCenter(getPosition() +  glm::vec3(0.f, .9f, 0.f));
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
    case Qt::Key_Space:
        m_jump = true;
        break;
    case Qt::Key_Minus:
    case Qt::Key_Underscore:
        m_offset += 1.f;
        if (m_offset > 30.f)
            m_offset = 30.f;
        m_camera->setOffset(m_offset);
        break;
    case Qt::Key_Plus:
    case Qt::Key_Equal:
        m_offset -= 1.f;
        if (m_offset < 0.f)
            m_offset = 0.f;
        m_camera->setOffset(m_offset);
        break;
    case Qt::Key_ParenRight:
    case Qt::Key_0:
        m_offset = 0.f;
        m_camera->setOffset(m_offset);
        break;
    case Qt::Key_ParenLeft:
    case Qt::Key_9:
        m_offset = 30.f;
        m_camera->setOffset(m_offset);
        break;
    case Qt::Key_Shift:
        m_forceAmt = 15.f;
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
    case Qt::Key_Space:
        m_jump = false;
        break;
    case Qt::Key_Shift:
        m_forceAmt = 8.f;
        break;
    default:
        break;
    }
}

void Player::handleCollision(Collision *col)
{
        setPosition(getPosition() + col->mtv);
        if (col->impulse.x > 0)
            m_canJump = true;

//        cout << glm::to_string(col->mtv) << endl;
//        vel.y = 0;
//        setVelocity(vel);
//        cout << "player" <<endl;
}


