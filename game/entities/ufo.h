#ifndef UFO_H
#define UFO_H

#include <QKeyEvent>
#include "movableentity.h"
#include "actioncamera.h"
#include "world.h"

class UFO : public MovableEntity
{
public:
    UFO(ActionCamera *camera, glm::vec3 pos, World *world);
    virtual ~UFO();

    void reset();

    virtual void onTick(float secs);
    virtual void onDrawOpaque(Graphics *g);
    virtual void onDrawTransparent(Graphics *g);
    virtual void handleCollision(Collision *col);

    void setCameraPos();

    // mouse events
    virtual void onMouseMoved(QMouseEvent *e, float deltaX, float deltaY);

    // key events
    virtual void onKeyPressed(QKeyEvent *e);
    virtual void onKeyReleased(QKeyEvent *e);

private:
    ActionCamera *m_camera;

    int m_wsad;
    bool m_up, m_down, m_beam;

    float m_offset;
    World *m_world;
};

#endif // UFO_H
