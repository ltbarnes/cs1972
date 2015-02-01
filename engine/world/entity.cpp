#include "entity.h"
#include "collisionshape.h"

#define GLM_FORCE_RADIANS
#include <glm/gtx/transform.hpp>

#include "printing.h"

Entity::Entity(glm::vec3 pos)
{
    m_pos = pos;
}

Entity::~Entity()
{
    foreach(RenderShape *rs, m_renderShapes)
        delete rs;
    foreach(CollisionShape *cs, m_collisionShapes)
        delete cs;
}

void Entity::setPosition(glm::vec3 pos)
{
    glm::vec3 diff = pos - m_pos;
    m_pos = pos;
    foreach(CollisionShape *cs, m_collisionShapes)
        cs->updatePos(diff);
}

glm::vec3 Entity::getPosition()
{
    return m_pos;
}

glm::vec3 Entity::getVelocity()
{
    return glm::vec3();
}

float Entity::getMass()
{
    return 0.f;
}

QList<CollisionShape *> Entity::getCollisionShapes()
{
    return m_collisionShapes;
}

void Entity::addRenderShape(RenderShape *rs)
{
    m_renderShapes.append(rs);
}

void Entity::addCollisionShape(CollisionShape *cs)
{
    cs->updatePos(getPosition());
    m_collisionShapes.append(cs);
}

void Entity::onTick(float)
{
}

void Entity::onDraw(Graphics *g)
{
    glm::mat4 posMat = glm::translate(glm::mat4(), m_pos);

    foreach(RenderShape *rs, m_renderShapes)
    {
//        cout << rs->texture.toStdString() << endl;
        g->setTexture(rs->texture, rs->repeatU, rs->repeatV);
        g->setColor(rs->color.r, rs->color.g, rs->color.b, rs->shininess);
        glm::mat4 trans = posMat * rs->trans;

        switch (rs->type)
        {
        case QUAD:
            g->drawQuad(trans);
            break;
        case CUBE:
            g->drawCube(trans);
            break;
        case CYLINDER:
            g->drawCyl(trans);
            break;
        case SPHERE:
            g->drawSphere(trans);
            break;
        }
    }
}

void Entity::bump(glm::vec3 amount)
{
    setPosition(getPosition() + amount);
}

Collision *Entity::collides(Entity *e)
{
    QList<CollisionShape *> cshapes = e->getCollisionShapes();
    glm::vec4 mtv;
    glm::vec4 mintv = glm::vec4(0, 0, 0, std::numeric_limits<float>::infinity());

    foreach(CollisionShape *cs1, m_collisionShapes)
    {
        foreach(CollisionShape *cs2, cshapes)
        {
            mtv = cs1->collides(cs2);
            if (mtv.w < mintv.w)
                mintv = mtv;
        }
    }
    Collision *col = NULL;

    if (mintv.w < std::numeric_limits<float>::infinity())
    {
        col = new Collision();
        col->e1 = this;
        col->e2 = e;
        col->mtv = glm::vec3(mintv);
        float m1 = this->getMass();
        float m2 = e->getMass();
        glm::vec3 v1 = this->getVelocity();
        glm::vec3 v2 = e->getVelocity();
        float mag2 = glm::dot(col->mtv, col->mtv);
        if (mag2 < 0.0001)
            col->impulse = glm::vec3(0.f);
        else
        {
            col->impulse = (v2 - v1) * ((m1 * m2 /**(1+cor)*/) / (m1 + m2));
            col->impulse = (glm::dot(col->impulse, col->mtv) / mag2) * col->mtv;
        }
    }
    return col;
}

