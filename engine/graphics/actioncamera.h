#ifndef ACTIONCAMERA_H
#define ACTIONCAMERA_H

#include "camera.h"

class ActionCamera : public Camera
{
public:
    ActionCamera();
    ~ActionCamera();

    float getOffset();

    void setCenter(glm::vec3 pos);
    void setOffset(float offset);

    void moveRelativeToLook(glm::vec3 dir);

private:
    glm::vec4 m_pos;//, m_offsetVec;
    float m_offset;
};

#endif // ACTIONCAMERA_H
