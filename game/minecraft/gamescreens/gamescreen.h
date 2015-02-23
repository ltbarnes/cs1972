#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include "screen.h"
#include "point3d.h"
#include "ally.h"
#include "enemy.h"

class MinecraftWorld;
class Player;
class MCChunkBuilder;

class GameScreen : public Screen
{
public:
    GameScreen(Application *parent);
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
    Point climb(Point p);
    Point descend(Point p);

    void scan();
    void placeAlly(Point p);

    MinecraftWorld *m_world;
    Player *m_player;
    MCChunkBuilder *m_cb;

    Point m_safety;
    int m_crawling, m_scanning;

    QList<Point> m_allies;
    QList<Enemy *> m_enemies;

    Point m_neighbors[8];
};

#endif // GAMESCREEN_H
