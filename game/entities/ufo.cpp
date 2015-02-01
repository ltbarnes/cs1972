#include "ufo.h"
#include "collisioncylinder.h"

UFO::UFO(ActionCamera *camera, glm::vec3 pos)
    : MovableEntity(pos, 100.f)
{
    m_camera = camera;

    CollisionShape *cs;
    cs = new CollisionCylinder(glm::vec3(), glm::vec3(16.f, 1.5f, 16.f));
    addCollisionShape(cs);

    // saucer
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
//        m_goalVel.x = 1.f;
        break;
    case Qt::Key_S:
//        m_goalVel.y = -1.f;
        break;
    case Qt::Key_A:
//        m_goalVel.z = -1.f;
        break;
    case Qt::Key_D:
//        m_goalVel.w = 1.f;
        break;
    case Qt::Key_Space:
//        m_jump = true;
        break;
    case Qt::Key_C:
//        m_crouch = !m_crouch;
        break;
    case Qt::Key_Shift:
//        m_sprint = true;
        break;
    case Qt::Key_CapsLock:
//        m_walk = true;
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


void UFO::onKeyReleased(QKeyEvent *e)
{
    switch (e->key())
    {
    case Qt::Key_W:
//        m_goalVel.x = 0.f;
        break;
    case Qt::Key_S:
//        m_goalVel.y = 0.f;
        break;
    case Qt::Key_A:
//        m_goalVel.z = 0.f;
        break;
    case Qt::Key_D:
//        m_goalVel.w = 0.f;
        break;
    case Qt::Key_Space:
//        m_jump = false;
        break;
    case Qt::Key_Shift:
//        m_sprint = false;
        break;
    case Qt::Key_CapsLock:
//        m_walk = false;
        break;
    default:
        break;
    }
}


