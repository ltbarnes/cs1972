#ifndef ELLIPSOID_H
#define ELLIPSOID_H

#include <glm.hpp>
#include "geometriccollisionmanager.h"

class Ellipsoid
{
public:
    Ellipsoid(glm::vec3 center, glm::vec3 radius);
    virtual ~Ellipsoid();

    glm::vec3 getCenter();
    glm::vec3 getRadius();
    glm::vec3 getDestination();

    void setCenter(glm::vec3 center);
    // temporary. should be calculated by velocity and tick time.
    void setDestination(glm::vec3 dest);

    float intersectRay(glm::vec3 p, glm::vec3 d);

    void intersectTriangle(Triangle tri, glm::vec3 p, glm::vec3 d, TriCollision *col);
    void handleCollision(TriCollision col);

    inline bool EQ(float a, float b) { return fabs(a - b) < 0.00001f; }

private:
    int findT(glm::vec3 p, glm::vec3 d, float *t1, float *t2);

    glm::vec3 m_center;
    glm::vec3 m_radius;
    glm::vec3 m_dest;
};

#endif // ELLIPSOID_H
