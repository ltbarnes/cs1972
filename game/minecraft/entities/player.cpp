#include "player.h"
#include "collisioncylinder.h"

#include <iostream>
using namespace std;
#include <glm/ext.hpp>

Player::Player(ActionCamera *camera, glm::vec3 pos, MinecraftWorld *world)
    : MovableEntity(pos)
{
    m_camera = camera;
    m_world = world;
    m_camera->setOffset(15.f);

    m_wsad = 0b0000;

    m_offset = 15.f;
    setMass(1.f);
    setVelocity(glm::vec3(0, -20, 0));

    m_up = false;
    m_jetMode = false;
    m_jump = false;
    m_canJump = false;

    CollisionShape *cs = new CollisionCylinder(glm::vec3(), glm::vec3(.99f, 1.98f, .99f), "player");
    cs->updatePos(pos);
    addCollisionShape(cs);

    RenderShape *rs = new RenderShape();
    rs->type = CYLINDER;
    rs->color = glm::vec3(0, .2f, 0);
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

void Player::grabAlly()
{
    setMass(5.f);
}

bool Player::hasAlly()
{
    return getMass() > 3.f;
}

int Player::getMode()
{
    if (m_jetMode && m_up)
        return 2;
    if (m_jetMode)
        return 1;
    return 0;
}


void Player::onTick(float secs)
{   

    float forceAmt = 8.f;
    if (m_jetMode)
        forceAmt += 7.f;

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
        applyForce(glm::vec3(0, (forceAmt *1.5f) * 2.f, 0));

    glm::vec4 look = m_camera->getLook();

    glm::vec3 thrust = glm::normalize(glm::vec3(look.x, 0.f, look.z)) * force.z;
    thrust += glm::normalize(glm::vec3(-look.z, 0.f, look.x)) * force.x;
    if (glm::length2(thrust) > 0.00001)
        thrust = glm::normalize(thrust) * forceAmt;
//    thrust.y = force.y;

    glm::vec3 vel = thrust - m_vel;
    if (m_jump && m_canJump)
        vel.y = forceAmt * 2.f;
    else if (m_jetMode)
        vel.y = 10.f * 2.f;
    else
        vel.y = 0.f;

    if (m_jetMode || !m_canJump)
        applyForce(vel);
    else
        applyImpulse(vel);

    m_canJump = false;

    // movable entity stuff
    m_vel += (m_force * secs / m_mass) + m_impulse * 1.f / m_mass;

    m_force = glm::vec3(0.f);
    m_impulse = glm::vec3(0.f);
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
    case Qt::Key_Space:
        if (m_jetMode)
            m_up = true;
        else
            m_jump = true;
        break;
    case Qt::Key_Minus:
    case Qt::Key_Underscore:
        m_offset += 1.f;
        if (m_offset > 15.f)
            m_offset = 15.f;
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
        m_offset = 15.f;
        m_camera->setOffset(m_offset);
        break;
    case Qt::Key_Shift:
        m_jetMode = true;
        if (m_jump)
        {
            m_up = true;
            m_jump = false;
        }
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
    case Qt::Key_Space:
        m_jump = false;
        m_up = false;
        break;
    case Qt::Key_Shift:
        m_jetMode = false;
        if (m_up)
        {
            m_jump = true;
            m_up = false;
        }
        break;
    default:
        break;
    }
}

void Player::handleCollision(Collision *col)
{
        setPosition(getPosition() + col->mtv);
        // velocity is already zeroed when col is made

        if (col->impulse.y > 0)
            m_canJump = true;
}


