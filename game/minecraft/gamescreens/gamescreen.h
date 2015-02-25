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
    void initEnemies(glm::vec3 p);
    void updateEnemies(glm::vec3 player);

    void display2D(Graphics *g);

    MinecraftWorld *m_world;
    Player *m_player;
    MCChunkBuilder *m_cb;

    Point m_safety;
    bool m_safetyStuck;
    int m_crawling, m_scanning;

    Point m_ally;
    QList<Enemy *> m_enemies;

    Point m_neighbors[8];

    Light *m_jetPackLight;
};

#endif // GAMESCREEN_H
