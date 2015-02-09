#ifndef GROUND_H
#define GROUND_H

#include "staticentity.h"

class Ground : public StaticEntity
{
public:
    Ground(glm::vec3 pos);
    virtual ~Ground();
};

#endif // GROUND_H
