#include "geometriccollisionmanager.h"
#include "ellipsoid.h"
#include "triangle.h"

GeometricCollisionManager::GeometricCollisionManager()
{
}


GeometricCollisionManager::~GeometricCollisionManager()
{
}


void GeometricCollisionManager::manage(World *world, float onTickSecs)
{
    int iterations = 4;

    QList<Ellipsoid*> ellis;
    QList<Triangle*> triangles;
    // ideally: ellis = world.getMovableEntities()
    // ellis.append(world->getPlayer().getCollisionShapes().

    // triangles = world->getTriangles();

    for (int i = 0; i < iterations; i++)
    {
        QList<TriCollision *> cols = detectEllipsoidTriangleCollisions(ellis, triangles);

        handleCollisions(cols);

        // delete collisions
        foreach (TriCollision *c, cols)
            delete c;
    }
}


QList<TriCollision* > GeometricCollisionManager::detectEllipsoidTriangleCollisions(
        QList<Ellipsoid *> ellis, QList<Triangle *> tris)
{
    float origT;
    QList<TriCollision *> cols;
    TriCollision col, best;

    foreach (Ellipsoid *elli, ellis)
    {
        best.e = elli;

        glm::vec3 d = (elli->getDestination() - elli->getCenter());
        origT = d.length();
        if (origT < 0.00001f)
            continue; // Ellipsoid hasn't moved

        best.t = origT;
        d = glm::normalize(d);
        best.dir = d;

        // put Ellipsoid and ray into sphere space
        glm::vec3 r = elli->getRadius();
        Ellipsoid e = Ellipsoid(elli->getCenter() / r, elli->getRadius() / r);
        glm::vec3 p = e.getCenter();
        d = d / r;

        glm::vec3 basis;
        // check for collisions with each triangle in the scene
        foreach (Triangle *orig, tris)
        {
            basis = glm::vec3(1 / r.x, 1 / r.y, 1 / r.z);
            e.intersectTriangle(orig->scale(basis), p, d, &col);
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
        if (best.e != NULL)
            cols.append(new TriCollision(best));
    }

    return cols;
}


void GeometricCollisionManager::handleCollisions(QList<TriCollision *> cols)
{
    float eps = 0.00001f;

    // handle collisions
    foreach (TriCollision *col, cols)
    {
        Ellipsoid *e = col->e;
        glm::vec3 &n = col->colNorm;
        glm::vec3 hit = e->getCenter() + col->dir * col->t + n * eps;

        glm::vec3 rem = col->dir * col->tMinus;

        if (col->type == PLANE)
        {
            rem = rem - (glm::dot(n, rem) / glm::dot(n, glm::vec3(0, 1, 0)) * glm::vec3(0, 1, 0));
            float tMinus = glm::length(rem - glm::dot(n, rem) * n);
            rem = glm::normalize(rem) * tMinus;
        }
        glm::vec3 perp = n * (glm::dot(n, rem) / glm::dot(n, n));
        glm::vec3 para = rem - perp;

        e->setCenter(hit + para);
    }
}


