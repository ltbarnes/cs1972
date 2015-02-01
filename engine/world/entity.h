#ifndef ENTITY_H
#define ENTITY_H

#include "graphics.h"

#define GLM_FORCE_RADIANS
#include <glm/gtx/transform.hpp>

class Entity;
class CollisionShape;

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
    Entity(glm::vec3 pos);
    virtual ~Entity();

    void setPosition(glm::vec3 pos);
    glm::vec3 getPosition();
    QList<CollisionShape *> getCollisionShapes();

    void addRenderShape(RenderShape *rs);
    void addCollisionShape(CollisionShape *cs);

    virtual void onTick(float secs);
    virtual void onDraw(Graphics *g);

    virtual glm::vec3 getVelocity();
    virtual float getMass();

    virtual Collision *collides(Entity *e);
    virtual void handleCollision(Entity *other, glm::vec3 mtv, glm::vec3 impulse) = 0;

    void bump(glm::vec3 amount);

private:
    glm::vec3 m_pos;

    QList<RenderShape *> m_renderShapes;
    QList<CollisionShape *> m_collisionShapes;

};

#endif // ENTITY_H
