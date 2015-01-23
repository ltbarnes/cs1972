#ifndef SCREEN_H
#define SCREEN_H

#include <QKeyEvent>
#include "graphics.h"
#include "camera.h"

class Screen
{
public:
    Screen()
    {
        // camera defaults set in Camera class
        m_camera = new Camera();
    }

    virtual ~Screen()
    {
        delete m_camera;
    }

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

    // return screen camera
    Camera *getCamera()
    {
        return m_camera;
    }

    // resize
    virtual void onResize(int w, int h)
    {
        m_camera->setAspectRatio(w * 1.f / h);
    }

protected:
    Camera *m_camera;
};

#endif // SCREEN_H
