#ifndef ITEM_H
#define ITEM_H

#include "movableentity.h"
#include "player.h"

class Item : public MovableEntity
{
public:
    Item(glm::vec3 pos, Player *player);
    virtual ~Item();

    virtual void onTick(float secs);

private:
    Player *m_player;
};

#endif // ITEM_H
