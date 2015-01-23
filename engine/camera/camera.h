#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera
{
public:
    Camera();
    virtual ~Camera();

    glm::mat4 getProjectionMatrix();
    glm::mat4 getViewMatrix();
    glm::vec4 getEye();

    void setAspectRatio(float a);
    void setEye(glm::vec4 &eye);
    void orientLook(glm::vec4 &eye, glm::vec4 &look, glm::vec4 &up);

    void moveForward(float dist);
    void moveBack(float dist);
    void moveLeft(float dist);
    void moveRight(float dist);

    void pitch(float degrees);
    void yaw(float degrees);
    void roll(float degrees);

private:
    void setCameraSpace();
    void setViewMatrix();
    void setProjectionMatrix();

    glm::vec4 m_u, m_v, m_w;
    glm::vec4 m_eye, m_look, m_up;

    glm::mat4 m_view, m_proj;

    // View variables
    float m_near, m_far, m_heightDegrees, m_aspectRatio;

};

#endif // CAMERA_H
