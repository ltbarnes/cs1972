#include "geometriccollisionmanager.h"
#include "ellipsoid.h"
#include "triangle.h"
#include "movableentity.h"
#include "world.h"

GeometricCollisionManager::GeometricCollisionManager()
{
}


GeometricCollisionManager::~GeometricCollisionManager()
{
}


void GeometricCollisionManager::manage(World *world, float)
{
    int iterations = 4;

    QList<MovableEntity*> ellis;
    QList<Triangle*> triangles;
//    ellis = world.getMovableEntities()
    ellis.append(world->getPlayer());

    // triangles = world->getTriangles();

    for (int i = 0; i < iterations; i++)
    {
        QList<TriCollision *> cols = detectTriangleCollisions(ellis, triangles);

        handleCollisions(cols);

        // delete collisions
        foreach (TriCollision *c, cols)
            delete c;
    }
}


QList<TriCollision* > GeometricCollisionManager::detectTriangleCollisions(
        QList<MovableEntity *> mes, QList<Triangle *> tris)
{
    float origT;
    QList<TriCollision *> cols;
    TriCollision col, best;

    foreach (MovableEntity *me, mes)
    {
        best.me = me;
        QList<CollisionShape*> shapes = me->getCollisionShapes();

        foreach (CollisionShape *cs, shapes)
        {
            glm::vec3 d = (me->getDestination() - cs->getPos());
            origT = d.length();
            if (origT < 0.00001f)
                continue; // Ellipsoid hasn't moved

            best.t = origT;
            best.dir = glm::normalize(d);

            // put Ellipsoid and ray into sphere space
            glm::vec3 r = cs->getDim();

            glm::vec3 basis = glm::vec3(1 / r.x, 1 / r.y, 1 / r.z);
            // check for collisions with each triangle in the scene
            foreach (Triangle *tri, tris)
            {
                cs->collidesTriangle(tri->scale(basis), d, &col);
                if (col.t < best.t)
                {
                    best.t = col.t;
                    best.tMinus = origT - col.t;
                    best.type = col.type;
                    best.colPoint = col.colPoint * r;   // back to world space
                    best.colNorm = col.colNorm * basis;     // back to world space
                    best.colNorm = glm::normalize(best.colNorm);
                }
            }
            // collision occured
            if (best.me != NULL)
                cols.append(new TriCollision(best));
            }
    }

    return cols;
}


void GeometricCollisionManager::handleCollisions(QList<TriCollision *> cols)
{
    float eps = 0.00001f;

    // handle collisions
    foreach (TriCollision *col, cols)
    {
        MovableEntity *me = col->me;
        glm::vec3 &n = col->colNorm;
        glm::vec3 hit = me->getPosition() + col->dir * col->t + n * eps;

        glm::vec3 rem = col->dir * col->tMinus;

        if (col->type == PLANE)
        {
            rem = rem - (glm::dot(n, rem) / glm::dot(n, glm::vec3(0, 1, 0)) * glm::vec3(0, 1, 0));
            float tMinus = glm::length(rem - glm::dot(n, rem) * n);
            rem = glm::normalize(rem) * tMinus;
        }
        glm::vec3 perp = n * (glm::dot(n, rem) / glm::dot(n, n));
        glm::vec3 para = rem - perp;

        me->setPosition(hit + para);
    }
}


