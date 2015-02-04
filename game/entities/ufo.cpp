#include "ufo.h"
#include "collisioncylinder.h"

#include "printing.h"

UFO::UFO(ActionCamera *camera, glm::vec3 pos, World *world)
    : MovableEntity(pos)
{
    m_camera = camera;
    m_world = world;

    m_offset = 30.f;
    m_beam = false;
    reset();

    // saucer
    CollisionShape *cs;
    cs = new CollisionCylinder(glm::vec3(), glm::vec3(16.f, 1.5f, 16.f), "hull");
    cs->setMass(100.f);
    addCollisionShape(cs);

    RenderShape *rs;
    rs = new RenderShape();
    rs->type = SPHERE;
    rs->color = glm::vec3(.5f);
    rs->shininess = 2048.f;
    rs->transparency = 1.f;
    rs->trans = glm::scale(glm::mat4(), glm::vec3(16.f, 1.6f, 16.f));
    rs->texture = "";
    rs->repeatU = 1.f;
    rs->repeatV = 1.f;
    addRenderShape(rs);

    // dome
    cs = new CollisionCylinder(glm::vec3(0, 2.f, 0), glm::vec3(6, .5f, 6), "ufo");
    cs->setMass(25.f);
    addCollisionShape(cs);

    rs = new RenderShape();
    rs->type = SPHERE;
    rs->color = glm::vec3(.7f, .7f, .9f);
    rs->shininess = 512.f;
    rs->transparency = 1.f;
    rs->trans = glm::translate(glm::mat4(), glm::vec3(0, .8, 0));
    rs->trans = glm::scale(rs->trans, glm::vec3(6.f, 3.f, 6.f));
    rs->texture = "";
    rs->repeatU = 1.f;
    rs->repeatV = 1.f;
    addRenderShape(rs);

    // beam
    cs = new CollisionCylinder(glm::vec3(0, -25, 0), glm::vec3(20, 50, 20), "beam");
    cs->setReactable(false);
    addCollisionShape(cs);

    rs = new RenderShape();
    rs->type = CONE;
    rs->color = glm::vec3(1.f, .3f, 0.f);
    rs->shininess = 0.f;
    rs->transparency = 0.3f;
    rs->trans = glm::translate(glm::mat4(), glm::vec3(0, -25.f, 0));
    rs->trans = glm::scale(rs->trans, glm::vec3(15.f, 50.f, 15.f));
    rs->texture = "";
    rs->repeatU = 1.f;
    rs->repeatV = 1.f;
    addRenderShape(rs);

}


UFO::~UFO()
{
}

void UFO::reset()
{
    m_wsad = 0b0000;
    m_up = false;
    m_down = false;
    m_camera->setOffset(m_offset);
}


void UFO::onTick(float secs)
{
    applyForce(glm::vec3(0.f, 9.95f, 0.f) * getMass());

    MovableEntity::onTick(secs);

    if (getPosition().y > 50.f)
        m_beam = false;

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

    m_rotation = glm::rotate(glm::mat4(), (std::abs(force.z) - glm::length(glm::vec2(m_vel.x, m_vel.z))) * force.z / -70000.f, glm::vec3(-look.z, 0, look.x));
    m_rotation = glm::rotate(m_rotation, (std::abs(force.x) - glm::length(glm::vec2(m_vel.x, m_vel.z))) * force.x / 70000.f, glm::vec3(look.x, 0, look.z));
//    cout << (std::abs(force.z) - )
}


void UFO::onDrawOpaque(Graphics *g)
{
    Entity::onDrawOpaque(g);
}

void UFO::onDrawTransparent(Graphics *g)
{
    if (m_beam)
    {
        Entity::onDrawTransparent(g);
    }
}


void UFO::setCameraPos()
{
    m_camera->setCenter(getPosition());
}


void UFO::handleCollision(Collision *col)
{
    MovableEntity::handleCollision(col);
    if (m_beam && col->c1->getID() == "beam" && col->c2->getID() != "ground")
    {
        MovableEntity *item = dynamic_cast<MovableEntity  *>(col->e2);
        glm::vec3 itemPos = item->getPosition();
        glm::vec3 pos = getPosition();
        glm::vec3 force = glm::vec3(pos.x, 0, pos.z) - glm::vec3(itemPos.x, 0, itemPos.z);
        float len = glm::length(force);
        float mag = max(0.f, (100.f - len * len) / 4.f);
        item->applyForce(glm::normalize(force) * mag);

        if (len < 5.f)
            item->applyForce(glm::vec3(0, (50.f - (pos.y - itemPos.y)) * .5f, 0));
    }
    if (m_beam && col->c1->getID() == "hull" && col->c2->getID() == "item")
    {
        m_world->setToDeleteMovable(dynamic_cast<MovableEntity  *>(col->e2));
    }

}


void UFO::onMouseMoved(QMouseEvent *, float deltaX, float deltaY)
{
    m_camera->yaw(deltaX / 10.f);
    m_camera->pitch(deltaY / 10.f);
}

void UFO::onKeyPressed(QKeyEvent *e)
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
        if (getPosition().y < 40.f || m_beam)
            m_beam = !m_beam;
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
    case Qt::Key_0:
        m_offset = 0.f;
        m_camera->setOffset(m_offset);
        break;
    case Qt::Key_9:
        m_offset = 30.f;
        m_camera->setOffset(m_offset);
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


