#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include "screen.h"
#include "obj.h"

class Player; // TEMP, SHOULD BE IN WORLD

class GameScreen : public Screen
{
public:
    GameScreen(Application *parent, int level);
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
    OBJ *m_level;
    Player *player;
    QString m_levelTexture;

};

#endif // GAMESCREEN_H
