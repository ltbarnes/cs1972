#ifndef ACTIONCAMERA_H
#define ACTIONCAMERA_H

#include "camera.h"

class ActionCamera : public Camera
{
public:
    ActionCamera();
    ~ActionCamera();

    void moveRelativeToLook(glm::vec3 move);
};

#endif // ACTIONCAMERA_H
