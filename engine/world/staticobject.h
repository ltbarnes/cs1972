#ifndef STATICOBJECT_H
#define STATICOBJECT_H

#include "staticentity.h"
#include "obj.h"

class StaticObject : public StaticEntity
{
public:
    StaticObject();
    virtual ~StaticObject();

    virtual void handleCollision(Collision *col);
};

#endif // STATICOBJECT_H
