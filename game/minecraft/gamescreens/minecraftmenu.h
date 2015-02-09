#ifndef MINECRAFTMENU_H
#define MINECRAFTMENU_H

#include "screen.h"

class MinecraftMenu : public Screen
{
public:
    MinecraftMenu(Application *parent);
    virtual ~MinecraftMenu();

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

#endif // MINECRAFTMENU_H
