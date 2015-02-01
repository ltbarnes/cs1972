#ifndef ITEM_H
#define ITEM_H

#include "movableentity.h"

class Item : public MovableEntity
{
public:
    Item(glm::vec3 pos);
    virtual ~Item();

//    virtual void onTick(float secs);
};

#endif // ITEM_H
