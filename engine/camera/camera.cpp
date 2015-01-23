#include "camera.h"

#define GLM_FORCE_RADIANS
#include <glm/gtx/rotate_vector.hpp>

Camera::Camera()
{
    glm::vec4 eye = glm::vec4(0, 0, 5, 0);
    glm::vec4 look = -eye;
    glm::vec4 up = glm::vec4(0, 1, 0, 0);
    orientLook(eye, look, up);

    // Projection Defaults
    m_heightDegrees = 60;
    m_aspectRatio = 1.0;
    m_near = 1.0;
    m_far = 50.0;
    setProjectionMatrix();
}

Camera::~Camera()
{
}

glm::mat4 Camera::getProjectionMatrix()
{
    return m_proj;
}

glm::mat4 Camera::getViewMatrix()
{
    return m_view;
}

glm::vec4 Camera::getEye()
{
    return m_eye;
}

void Camera::setAspectRatio(float a)
{
    m_aspectRatio = a;
    setProjectionMatrix();
}

void Camera::setEye(glm::vec4 &eye)
{
    orientLook(eye, m_look, m_up);
}

void Camera::orientLook(glm::vec4 &eye, glm::vec4 &look, glm::vec4 &up)
{
    // Camera vecs
    m_eye = eye;
    m_look = look;
    m_up = up;

    setCameraSpace();
    setViewMatrix();
}


void Camera::moveForward(float dist)
{
    m_eye += glm::normalize(glm::vec4(m_look.x, 0, m_look.z, 0)) * dist;
    setViewMatrix();
}

void Camera::moveBack(float dist)
{
    m_eye += glm::normalize(glm::vec4(-m_look.x, 0, -m_look.z, 0)) * dist;
    setViewMatrix();
}

void Camera::moveLeft(float dist)
{
    m_eye += glm::normalize(glm::vec4(m_look.z, 0, -m_look.x, 0)) * dist;
    setViewMatrix();
}

void Camera::moveRight(float dist)
{
    m_eye += glm::normalize(glm::vec4(-m_look.z, 0, m_look.x, 0)) * dist;
    setViewMatrix();
}

void Camera::pitch(float degrees)
{

    m_look = glm::rotate(m_look, glm::radians(degrees), glm::cross(glm::vec3(m_up), glm::vec3(m_look)));
    setCameraSpace();
    setViewMatrix();
}

void Camera::yaw(float degrees)
{
    float radians = glm::radians(degrees);
    glm::vec3 vec = glm::vec3(0.f, -1.f, 0.f);
    m_look = glm::rotate(m_look, radians, vec);
    m_up = glm::rotate(m_up, radians, vec);
    setCameraSpace();
    setViewMatrix();
}

void Camera::roll(float degrees)
{
    m_up = glm::rotate(m_up, glm::radians(degrees), glm::vec3(m_look));
    setCameraSpace();
    setViewMatrix();
}

void Camera::setCameraSpace()
{
    // Camera coordinate space
    m_look = glm::normalize(m_look);
    m_w = -m_look;
    m_v = glm::normalize(m_up - (glm::dot(m_up, m_w) * m_w));
    m_u = glm::vec4(
                glm::normalize(
                        glm::cross(glm::vec3(m_v.x, m_v.y, m_v.z),
                                   glm::vec3(m_w.x, m_w.y, m_w.z))),
                    0);
    m_up = m_v;
}

void Camera::setViewMatrix()
{
    // View Matrices
    glm::mat4 trans = glm::mat4();
    trans[3][0] = -m_eye[0];
    trans[3][1] = -m_eye[1];
    trans[3][2] = -m_eye[2];

    glm::mat4 rot = glm::mat4(m_u.x, m_u.y, m_u.z, 0.0,
                              m_v.x, m_v.y, m_v.z, 0.0,
                              m_w.x, m_w.y, m_w.z, 0.0,
                               0.0,   0.0,   0.0,  1.0);
    rot = glm::transpose(rot);
    m_view = rot * trans;
}

void Camera::setProjectionMatrix()
{
    // Projection Matrices
    float h = 2.0 * m_far * glm::tan(glm::radians(m_heightDegrees / 2.0));
    float w = m_aspectRatio * h;

    glm::mat4 scale = glm::mat4(1.0 / (w / 2.0),       0.0,          0.0,     0.0,
                                     0.0,        1.0 / (h / 2.0),    0.0,     0.0,
                                     0.0,              0.0,       1.0 / m_far,  0.0,
                                     0.0,              0.0,          0.0,     1.0);
    scale = glm::transpose(scale);

    float c = -m_near / m_far;
    glm::mat4 perspective = glm::mat4(1.0,   0.0,      0.0,         0.0,
                                      0.0,   1.0,      0.0,         0.0,
                                      0.0,   0.0, -1.0/(1.0+c),  c/(1.0+c),
                                      0.0,   0.0,     -1.0,         0.0);
    perspective = glm::transpose(perspective);
    m_proj = perspective * scale;
}



