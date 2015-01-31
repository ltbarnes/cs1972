#ifndef ENTITY_H
#define ENTITY_H

#include "graphics.h"

class Entity;

struct Collision
{
    Entity *e1;
    Entity *e2;
    glm::vec3 mtv;
    glm::vec3 impulse;
};

class Entity
{
public:
    Entity(glm::vec3 pos, glm::vec3 dim);
    ~Entity();

    glm::vec3 getPosition();
    glm::vec3 getDimension();

    void addRenderShape(RenderShape *rs);
//    void addCollisionShape(CollisionShape *cs);

    virtual void onTick(float secs);
    virtual void onDraw(Graphics *g);

    virtual Collision *collides(Entity *e);
    virtual void handleCollision(Entity *other, glm::vec3 mtv, glm::vec3 impulse) = 0;

    void bump(glm::vec3 amount);

//    static

protected:
    glm::vec3 m_pos, m_dim;

    QList<RenderShape *> m_renderShapes;
//    QList<CollisionShape *> m_collisionShapes;

};

#endif // ENTITY_H
