#ifndef CUBE_H
#define CUBE_H

#include "shape.h"

class Cube : public Shape
{
public:
    Cube();
    ~Cube();

    virtual void calcVerts();
};

#endif // CUBE_H
