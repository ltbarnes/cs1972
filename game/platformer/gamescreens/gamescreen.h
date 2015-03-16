#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include "screen.h"
#include "objecthandler.h"
#include "platformerworld.h"
#include "ellipsoid.h"
#include "navmeshhandler.h"

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
    QString m_levelTexture;
    OBJ *m_level;
    NavMeshHandler *m_nmh;

    PlatformerWorld *m_world;

    bool m_drawEllipsoid;
    bool m_drawPoint;
    bool m_mouseDown;
    bool m_drawNavMesh;

    Ellipsoid* m_ellipsoid;
    glm::vec3 m_point;

};

#endif // GAMESCREEN_H
