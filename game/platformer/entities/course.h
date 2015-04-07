#ifndef COURSE_H
#define COURSE_H

#include "graphics.h"
#include <QList>

class Course
{
public:
    Course();
    virtual ~Course();

    void draw(Graphics *g);
    ObjectsInfo *getObjectInfo();

private:
    void addCylinder(glm::mat4 trans);

    QList<RenderShape *> m_cylinders;

};

#endif // COURSE_H
