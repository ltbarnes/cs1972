#ifndef CREATESCREEN_H
#define CREATESCREEN_H

#include "screen.h"

class MinecraftWorld;
class MCChunkBuilder;
class ActionCamera;

class CreateScreen : public Screen
{
public:
    CreateScreen(Application *parent);
    virtual ~CreateScreen();

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
    MCChunkBuilder *m_cb;

    ActionCamera *m_cam;

    int m_movement;
    bool m_moveSlow;
    glm::vec3 m_pos;
};

#endif // CREATESCREEN_H
