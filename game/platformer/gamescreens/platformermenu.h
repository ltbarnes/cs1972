#ifndef PLATFORMERMENU_H
#define PLATFORMERMENU_H

#include "screen.h"
#include "button.h"

class PlatformerMenu : public Screen
{
public:
    PlatformerMenu(Application *parent);
    virtual ~PlatformerMenu();

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

    virtual void onResize(int w, int h);

private:
    Button *m_buttonEasy;
    Button *m_buttonHard;
    Button *m_buttonIsland;

    glm::mat4 m_cursor;
    int m_level;
};

#endif // PLATFORMERMENU_H
