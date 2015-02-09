#ifndef MENUSCREEN_H
#define MENUSCREEN_H

#include "screen.h"

class MenuScreen : public Screen
{
public:
    MenuScreen(Application *parent);
    virtual ~MenuScreen();

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
};

#endif // MENUSCREEN_H
