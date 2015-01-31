#include "entity.h"

#define GLM_FORCE_RADIANS
#include <glm/gtx/transform.hpp>

Entity::Entity(glm::vec3 pos, glm::vec3 dim)
{
    m_pos = pos;
    m_dim = dim;
}

Entity::~Entity()
{
    foreach(RenderShape *rs, m_renderShapes)
        delete rs;
//    foreach(CollisionShape *cs, m_collisionShapes)
//        delete cs;
}

glm::vec3 Entity::getPosition()
{
    return m_pos;
}

glm::vec3 Entity::getDimension()
{
    return m_dim;
}

void Entity::addRenderShape(RenderShape *rs)
{
    m_renderShapes.append(rs);
}

void Entity::onTick(float)
{
}

void Entity::onDraw(Graphics *g)
{
    glm::mat4 posMat = glm::translate(glm::mat4(), m_pos);

    foreach(RenderShape *rs, m_renderShapes)
    {
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

Collision *Entity::collides(Entity *e)
{
    Collision *col = NULL;
    glm::vec3 pos = e->getPosition();
    glm::vec3 dim = e->getDimension();

//    foreach(CollisionShape *cs, m_collisionShapes)
//    {

//    }

    return col;
}

void Entity::bump(glm::vec3 amount)
{
    m_pos += amount;
}

