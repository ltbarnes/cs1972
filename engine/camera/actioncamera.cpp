#include "actioncamera.h"

ActionCamera::ActionCamera()
{
    m_pos = glm::vec4();
    m_offset = 0.f;
}

ActionCamera::~ActionCamera()
{
}


float ActionCamera::getOffset()
{
    return m_offset;
}

void ActionCamera::setCenter(glm::vec3 pos)
{
    m_pos = glm::vec4(pos, 1.f);
    glm::vec4 eye = m_pos - m_look * m_offset;
    orientLook(eye, m_look, m_up);
}


void ActionCamera::setOffset(float offset)
{
    m_offset = offset;
    glm::vec4 eye = m_pos - m_look * m_offset;
    orientLook(eye, m_look, m_up);
}


void ActionCamera::moveRelativeToLook(glm::vec3 dir)
{
    moveAlongU(dir.x);
    moveAlongUp(dir.y);
    moveAlongLook(dir.z);
}
