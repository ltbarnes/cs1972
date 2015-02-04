#include "beam.h"

Beam::Beam(glm::vec3 pos)
{
}


Beam::~Beam()
{
}


void Beam::handleCollision(Entity *other, glm::vec3, glm::vec3)
{
    glm::vec3 pos = other->getPosition();

}

