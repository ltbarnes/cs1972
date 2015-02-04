#ifndef ENDGAMESCREEN_H
#define ENDGAMESCREEN_H

#include "screen.h"

class EndGameScreen : public Screen
{
public:
    EndGameScreen(Application *parent, bool won);
    virtual ~EndGameScreen();

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
    bool m_won;

};

#endif // ENDGAMESCREEN_H
