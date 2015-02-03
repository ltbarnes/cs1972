#ifndef UFO_H
#define UFO_H

#include <QKeyEvent>
#include "movableentity.h"
#include "actioncamera.h"

class UFO : public MovableEntity
{
public:
    UFO(ActionCamera *camera, glm::vec3 pos);
    virtual ~UFO();

    virtual void onTick(float secs);
    virtual void onDraw(Graphics *g);

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
};

#endif // UFO_H
