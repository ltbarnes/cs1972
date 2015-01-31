#ifndef GAMEWORLD_H
#define GAMEWORLD_H

#include "world.h"

class GameWorld : public World
{
public:
    GameWorld();
    ~GameWorld();

    virtual void onTick(float secs);
    virtual void onDraw(Graphics *g);

private:
    Light *m_light;
};

#endif // GAMEWORLD_H
