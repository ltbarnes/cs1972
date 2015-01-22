#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera
{
public:
    Camera();
//    virtual ~Camera();

//    void orientLook(glm::vec4 &eye, glm::vec4 &look, glm::vec4 &up);


private:
//    float m_pitch, m_yaw, m_roll;
    glm::vec4 m_eye, m_look, m_up;

    glm::mat4 m_view, m_proj;
};

#endif // CAMERA_H
