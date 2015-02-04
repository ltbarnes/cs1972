#ifndef ITEM_H
#define ITEM_H

#include "movableentity.h"
#include "player.h"
#include "application.h"

class Item : public MovableEntity
{
public:
    Item(glm::vec3 pos, Player *player, Application *app);
    virtual ~Item();

    virtual void onTick(float secs);

private:
    Player *m_player;
    Application *m_app;
};

#endif // ITEM_H
