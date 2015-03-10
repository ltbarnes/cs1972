#ifndef GEOMETRICCOLLISIONMANAGER_H
#define GEOMETRICCOLLISIONMANAGER_H

#include "manager.h"

class Ellipsoid;
class Triangle;

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
    Ellipsoid *e;

    TriCollision() : t(INFINITY) {}
};

class GeometricCollisionManager : public Manager
{
public:
    GeometricCollisionManager();
    virtual ~GeometricCollisionManager();

    virtual void manage(World *world, float onTickSecs);

    QList<TriCollision *> detectEllipsoidTriangleCollisions(
            QList<Ellipsoid *> ellis, QList<Triangle *> tris);

    void handleCollisions(QList<TriCollision *> cols);
};

#endif // GEOMETRICCOLLISIONMANAGER_H
