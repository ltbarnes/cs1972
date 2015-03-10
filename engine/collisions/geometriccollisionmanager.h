#ifndef GEOMETRICCOLLISIONMANAGER_H
#define GEOMETRICCOLLISIONMANAGER_H

#include "manager.h"
#include "collisionshape.h"
#include "movableentity.h"

enum CollisionType
{
    PLANE, EDGE, VERTEX
};

struct TriCollision
{
    float t;
    float tMinus;
    CollisionType type;
    glm::vec3 dir;
    glm::vec3 colPoint;
    glm::vec3 colNorm;
    MovableEntity *me;

    TriCollision() : t(INFINITY) {}
};

class GeometricCollisionManager : public Manager
{
public:
    GeometricCollisionManager();
    virtual ~GeometricCollisionManager();

    virtual void manage(World *world, float secs);

    QList<TriCollision *> detectTriangleCollisions(
            QList<MovableEntity *> mes, QList<Triangle *> tris);

    void handleCollisions(QList<TriCollision *> cols);
};

#endif // GEOMETRICCOLLISIONMANAGER_H
