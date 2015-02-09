#ifndef PLAYER_H
#define PLAYER_H

#include "movableentity.h"
#include "actioncamera.h"
#include "world.h"
#include <QMouseEvent>

class Player : public MovableEntity
{
public:
    Player(ActionCamera *camera, glm::vec3 pos, World *world);
    virtual ~Player();

    virtual void onTick(float secs);
    virtual void onDrawOpaque(Graphics *g);
    virtual void onDrawTransparent(Graphics *g);

    void setCameraPos();

    // mouse events
    virtual void onMouseMoved(QMouseEvent *e, float deltaX, float deltaY);

    // key events
    virtual void onKeyPressed(QKeyEvent *e);
    virtual void onKeyReleased(QKeyEvent *e);

private:
    ActionCamera *m_camera;

    int m_wsad;
    bool m_up, m_down;

    float m_offset;
    World *m_world;

    float m_forceAmt;
};

#endif // PLAYER_H
