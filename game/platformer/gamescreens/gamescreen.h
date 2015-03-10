#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include "screen.h"
#include "objecthandler.h"
#include "platformerworld.h"

class PlatformerPlayer; // TEMP, SHOULD BE IN WORLD

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
    ObjectHandler *m_oh;
    OBJ *m_level;
    QString m_levelTexture;

    PlatformerWorld *m_world;
    PlatformerPlayer *player;

};

#endif // GAMESCREEN_H
