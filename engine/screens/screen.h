#ifndef SCREEN_H
#define SCREEN_H

#include <QKeyEvent>
#include "graphics.h"
#include "camera.h"

class Screen
{
public:
    virtual ~Screen() = 0;

    // update and render
    virtual void onTick(float secs) = 0;
    virtual void onRender(Graphics *g) = 0;

    // mouse events
    virtual void onMousePressed(QMouseEvent *e) = 0;
    virtual void onMouseMoved(QMouseEvent *e) = 0;
    virtual void onMouseReleased(QMouseEvent *e) = 0;

    virtual void onMouseDragged(QMouseEvent *e) = 0;
    virtual void onMouseWheel(QWheelEvent *e) = 0;

    // key events
    virtual void onKeyPressed(QKeyEvent *e) = 0;
    virtual void onKeyReleased(QKeyEvent *e) = 0;

    // resize
    virtual void onResize(int w, int h) = 0;

protected:
    Camera *m_camera;
};

#endif // SCREEN_H
