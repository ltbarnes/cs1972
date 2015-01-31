#ifndef PLAYER_H
#define PLAYER_H

#include <QKeyEvent>
#include "movableentity.h"
#include "actioncamera.h"

class Player : public MovableEntity
{
public:
    Player(ActionCamera *camera, glm::vec3 pos);
    ~Player();

    virtual void onTick(float secs);
    virtual void onDraw(Graphics *g);

    // key events
    virtual void onKeyPressed(QKeyEvent *e);
    virtual void onKeyReleased(QKeyEvent *e);

private:
    ActionCamera *m_camera;

    glm::vec3 m_eye;

    glm::vec4 m_goalVel;

    bool m_jump;
    bool m_crouch;
    bool m_walk;
    bool m_sprint;
};

#endif // PLAYER_H
