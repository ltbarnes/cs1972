#include "course.h"

#define GLM_FORCE_RADIANS
#include <glm/gtc/constants.hpp>
#include <glm/gtx/transform.hpp>

Course::Course()
{
    float diagWidth = 14.14f;
    float width = 20.f;

    // diag "X"
    glm::mat4 trans;
//    trans = glm::rotate(glm::mat4(), glm::half_pi<float>(), glm::vec3(1,0,1)) *
//            glm::scale(glm::mat4(), glm::vec3(diagWidth, 183.848f, diagWidth));
//    addCylinder(trans);

    trans = glm::rotate(glm::mat4(), glm::half_pi<float>(), glm::vec3(-1,0,1)) *
            glm::scale(glm::mat4(), glm::vec3(diagWidth, 183.848f, diagWidth));
    addCylinder(trans);

//    // horizontal
//    trans = glm::translate(glm::mat4(), glm::vec3(80,0,-60)) *
//            glm::rotate(glm::mat4(), glm::half_pi<float>(), glm::vec3(0,0,1)) *
//            glm::scale(glm::mat4(), glm::vec3(diagWidth, 40, width));
//    addCylinder(trans);

//    trans = glm::translate(glm::mat4(), glm::vec3(80,0,60)) *
//            glm::rotate(glm::mat4(), glm::half_pi<float>(), glm::vec3(0,0,1)) *
//            glm::scale(glm::mat4(), glm::vec3(diagWidth, 40, width));
//    addCylinder(trans);

//    trans = glm::translate(glm::mat4(), glm::vec3(-80,0,-60)) *
//            glm::rotate(glm::mat4(), glm::half_pi<float>(), glm::vec3(0,0,1)) *
//            glm::scale(glm::mat4(), glm::vec3(diagWidth, 40, width));
//    addCylinder(trans);

//    trans = glm::translate(glm::mat4(), glm::vec3(-80,0,60)) *
//            glm::rotate(glm::mat4(), glm::half_pi<float>(), glm::vec3(0,0,1)) *
//            glm::scale(glm::mat4(), glm::vec3(diagWidth, 40, width));
//    addCylinder(trans);

//    // vertical
//    trans = glm::translate(glm::mat4(), glm::vec3(130,0,0)) *
//            glm::rotate(glm::mat4(), glm::half_pi<float>(), glm::vec3(1,0,0)) *
//            glm::scale(glm::mat4(), glm::vec3(width, 60, diagWidth));
//    addCylinder(trans);

//    trans = glm::translate(glm::mat4(), glm::vec3(-130,0,0)) *
//            glm::rotate(glm::mat4(), glm::half_pi<float>(), glm::vec3(1,0,0)) *
//            glm::scale(glm::mat4(), glm::vec3(width, 60, diagWidth));
//    addCylinder(trans);

//    // diags
//    trans = glm::translate(glm::mat4(), glm::vec3(115,0,-45)) *
//            glm::rotate(glm::mat4(), glm::half_pi<float>(), glm::vec3(-1,0,1)) *
//            glm::scale(glm::mat4(), glm::vec3(diagWidth, 56.57f, diagWidth));
//    addCylinder(trans);

//    trans = glm::translate(glm::mat4(), glm::vec3(115,0,45)) *
//            glm::rotate(glm::mat4(), glm::half_pi<float>(), glm::vec3(1,0,1)) *
//            glm::scale(glm::mat4(), glm::vec3(diagWidth, 56.57f, diagWidth));
//    addCylinder(trans);

//    trans = glm::translate(glm::mat4(), glm::vec3(-115,0,-45)) *
//            glm::rotate(glm::mat4(), glm::half_pi<float>(), glm::vec3(1,0,1)) *
//            glm::scale(glm::mat4(), glm::vec3(diagWidth, 56.57f, diagWidth));
//    addCylinder(trans);

//    trans = glm::translate(glm::mat4(), glm::vec3(-115,0,45)) *
//            glm::rotate(glm::mat4(), glm::half_pi<float>(), glm::vec3(-1,0,1)) *
//            glm::scale(glm::mat4(), glm::vec3(diagWidth, 56.57f, diagWidth));
//    addCylinder(trans);

}


Course::~Course()
{
    foreach (RenderShape *rs, m_cylinders)
        delete rs;
}


void Course::addCylinder(glm::mat4 trans)
{
    RenderShape *rs;
    rs = new RenderShape();
    rs->type = CYLINDER;
    rs->color = glm::vec3(.3f);
    rs->transparency = 1.f;
    rs->shininess = 0.f;
    rs->trans = trans;
    rs->inv = glm::inverse(trans);
    rs->texture = "";
    rs->repeatU = 1.f;
    rs->repeatV = 1.f;

    m_cylinders.append(rs);
}


void Course::draw(Graphics *g)
{
    foreach (RenderShape *rs, m_cylinders)
    {
        g->setTexture(rs->texture, rs->repeatU, rs->repeatV);
        g->setColor(rs->color.r, rs->color.g, rs->color.b, rs->transparency, rs->shininess);
        g->drawCyl(rs->trans);
    }
}


ObjectsInfo *Course::getObjectInfo()
{
    ObjectsInfo *info;
    info = new ObjectsInfo();

    foreach(RenderShape *rs, m_cylinders)
    {
        info->invs.append(rs->inv);
        info->colors.append(glm::vec4(rs->color, rs->transparency));
        info->shapeType.append(rs->type);
    }

    return info;
}


