#ifndef PLAYER_H
#define PLAYER_H

#include "movableentity.h"
#include "actioncamera.h"
#include <QMouseEvent>

class Player : public MovableEntity
{
public:
    Player(ActionCamera *camera, glm::vec3 pos);
    virtual ~Player();

    virtual void onTick(float secs);
//    virtual void onDrawOpaque(Graphics *g);
//    virtual void onDrawTransparent(Graphics *g);

    void setCameraPos();

//    void grabAlly();
//    bool hasAlly();
//    void decreaseHealth(int amount);
//    int getHealth();

//    int getMode();

    // mouse events
    virtual void onMouseMoved(QMouseEvent *e, float deltaX, float deltaY);

    // key events
    virtual void onKeyPressed(QKeyEvent *e);
    virtual void onKeyReleased(QKeyEvent *e);

//    virtual void handleCollision(Collision *col);

private:
    ActionCamera *m_camera;

    int m_wsad;
    bool m_up, m_down;
//    bool m_canJump, m_jump;
};

#endif // PLAYER_H
