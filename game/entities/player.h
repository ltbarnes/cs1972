#ifndef PLAYER_H
#define PLAYER_H

#include <QKeyEvent>
#include "movableentity.h"
#include "actioncamera.h"

class Player : public MovableEntity
{
public:
    Player(ActionCamera *camera, glm::vec3 pos);
    virtual ~Player();

    void reset();

    virtual void onTick(float secs);
    virtual void onDraw(Graphics *g);

    void setCameraPos();

    virtual void handleCollision(Entity *other, glm::vec3 mtv, glm::vec3 impulse);

    // mouse events
    virtual void onMouseMoved(QMouseEvent *e, float deltaX, float deltaY);

    // key events
    virtual void onKeyPressed(QKeyEvent *e);
    virtual void onKeyReleased(QKeyEvent *e);

private:
    ActionCamera *m_camera;

    float m_eyeHeight;

    glm::vec4 m_goalVel;

    bool m_jump, m_canJump;
    bool m_crouch;
    bool m_walk;
    bool m_sprint;
};

#endif // PLAYER_H
