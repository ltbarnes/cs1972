#ifndef GAMEOVERSCREEN_H
#define GAMEOVERSCREEN_H

#include "screen.h"

class GameOverScreen : public Screen
{
public:
    GameOverScreen(Application *parent, int gameStatus);
    virtual ~GameOverScreen();

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
    int m_gameStatus;

};

#endif // GAMEOVERSCREEN_H
