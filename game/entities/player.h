#ifndef PLAYER_H
#define PLAYER_H

#include <QKeyEvent>
#include "graphics.h"
#include "actioncamera.h"

class Player //: public Entity
{
public:
    Player(ActionCamera *camera);
    ~Player();

    virtual void onTick(float secs);
    virtual void onDraw(Graphics *g);

    // key events
    virtual void onKeyPressed(QKeyEvent *e);
    virtual void onKeyReleased(QKeyEvent *e);

private:
    ActionCamera *m_camera;

    glm::vec4 m_goalVel;
    glm::vec3 m_currVel;

    float m_jumpVelocity;
    bool m_jump;
    bool m_crouch;
    bool m_walk;
    bool m_sprint;
};

#endif // PLAYER_H
