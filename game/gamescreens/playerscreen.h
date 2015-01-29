#ifndef PLAYERSCREEN_H
#define PLAYERSCREEN_H

#include "screen.h"
#include "player.h"
#include "world.h"

class PlayerScreen : public Screen
{
public:
    PlayerScreen();
    ~PlayerScreen();

    // update and render
    virtual void onTick(float secs);
    virtual void onRender(Graphics *g);

    // mouse events
    virtual void onMousePressed(QMouseEvent *e);
    virtual void onMouseMoved(QMouseEvent *e, float deltaX, float deltaY);
    virtual void onMouseReleased(QMouseEvent *e);

    virtual void onMouseDragged(QMouseEvent *e, float deltaX, float deltaY);
    virtual void onMouseWheel(QWheelEvent *e);

    // key events
    virtual void onKeyPressed(QKeyEvent *e);
    virtual void onKeyReleased(QKeyEvent *e);

private:
    Player *m_player;
    World *m_world;

    int m_movementDir;
    float m_jumpVelocity;
    bool m_crouch;
    bool m_walk;
    bool m_sprint;

};

#endif // PLAYERSCREEN_H
