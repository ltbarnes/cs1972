#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include "screen.h"
#include "point3d.h"

class MinecraftWorld;
class Player;
class MCChunkBuilder;

class GameScreen : public Screen
{
public:
    GameScreen(Application *parent);
    virtual ~GameScreen();

    // update and render
    virtual void onTick(float secs);
    virtual void onRender(Graphics *g);

    // mouse events
    virtual void onMousePressed(QMouseEvent *e);
    virtual void onMouseMoved(QMouseEvent *e, float deltaX, float deltaY);
    virtual void onMouseReleased(QMouseEvent *);

    virtual void onMouseDragged(QMouseEvent *e, float deltaX, float deltaY);
    virtual void onMouseWheel(QWheelEvent *e);

    // key events
    virtual void onKeyPressed(QKeyEvent *e);
    virtual void onKeyReleased(QKeyEvent *e);

private:
    MinecraftWorld *m_world;
    Player *m_player;
    MCChunkBuilder *m_cb;

    Point m_safety;



    float totalSecs;
    int count;
};

#endif // GAMESCREEN_H
