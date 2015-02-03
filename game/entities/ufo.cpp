#include "ufo.h"
#include "collisioncylinder.h"

#include "printing.h"

UFO::UFO(ActionCamera *camera, glm::vec3 pos)
    : MovableEntity(pos, 100.f)
{
    m_camera = camera;

    m_wsad = 0b0000;
    m_up = false;
    m_down = false;
    m_beam = false;

    // saucer
    CollisionShape *cs;
    cs = new CollisionCylinder(glm::vec3(), glm::vec3(16.f, 1.5f, 16.f));
    addCollisionShape(cs);

    RenderShape *rs;
    rs = new RenderShape();
    rs->type = SPHERE;
    rs->color = glm::vec3(.5f);
    rs->shininess = 2048.f;
    rs->trans = glm::scale(glm::mat4(), glm::vec3(16.f, 1.6f, 16.f));
    rs->texture = "";
    rs->repeatU = 1.f;
    rs->repeatV = 1.f;
    addRenderShape(rs);

    // dome
    cs = new CollisionCylinder(glm::vec3(0, .8f, 0), glm::vec3(6, 3, 6));
    addCollisionShape(cs);

    rs = new RenderShape();
    rs->type = SPHERE;
    rs->color = glm::vec3(.7f, .7f, .9f);
    rs->shininess = 512.f;
    rs->trans = glm::translate(glm::mat4(), glm::vec3(0, .8, 0));
    rs->trans = glm::scale(rs->trans, glm::vec3(6.f, 3.f, 6.f));
    rs->texture = "";
    rs->repeatU = 1.f;
    rs->repeatV = 1.f;
    addRenderShape(rs);
}


UFO::~UFO()
{
}


void UFO::onTick(float secs)
{
    applyForce(glm::vec3(0.f, 9.95f, 0.f) * getMass());

    MovableEntity::onTick(secs);

    glm::vec3 force = glm::vec3(0.f);
    if (m_wsad & 0b1000)
        force.z += 100.f;
    if (m_wsad & 0b0100)
        force.z -= 100.f;
    if (m_wsad & 0b0010)
        force.x -= 100.f;
    if (m_wsad & 0b0001)
        force.x += 100.f;
    if (m_up)
        force.y += 100.f;
    if (m_down)
        force.y -= 100.f;

    glm::vec4 look = m_camera->getLook();

    glm::vec3 thrust = glm::normalize(glm::vec3(look.x, 0.f, look.z)) * force.z;
    thrust += glm::normalize(glm::vec3(-look.z, 0.f, look.x)) * force.x;
    thrust.y = force.y;

    applyForce((thrust - m_vel) * 100.f);
}


void UFO::onDraw(Graphics *g)
{
    Entity::onDraw(g);
}


void UFO::setCameraPos()
{
    m_camera->setCenter(getPosition());
}


//void UFO::handleCollision(Entity *other, glm::vec3 mtv, glm::vec3 impulse)
//{
//    MovableEntity::handleCollision(other, mtv, impulse);
//}


void UFO::onMouseMoved(QMouseEvent *, float deltaX, float deltaY)
{
    m_camera->yaw(deltaX / 10.f);
    m_camera->pitch(deltaY / 10.f);
}

void UFO::onKeyPressed(QKeyEvent *e)
{
    float offset;

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
        break;
    case Qt::Key_Minus:
    case Qt::Key_Underscore:
        offset = m_camera->getOffset();
        offset += 1.f;
        if (offset > 25.f)
            offset = 25.f;
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


void UFO::onKeyReleased(QKeyEvent *e)
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
        break;
    default:
        break;
    }
}


