#ifndef NAVMESHHANDLER_H
#define NAVMESHHANDLER_H

#include <QList>
#include "triangle.h"
#include "obj.h"

class NavMeshHandler
{
public:
    NavMeshHandler();
    virtual ~NavMeshHandler();

    void setObject(OBJ *obj);
    bool hasObject();
    QList<Triangle*> *getTriangleList();

    void createVBO();

    void draw(glm::mat4 trans);
    void drawLines(glm::mat4 trans);

private:
    void fillVertex(int *index, GLfloat *data, glm::vec3 v);

    QList<Triangle *> m_mesh;
    OBJ *m_obj;

    GLuint m_shader;
    GLuint m_vaoID;
    GLuint m_vboID;
    int m_numVerts;

};

#endif // NAVMESHHANDLER_H
