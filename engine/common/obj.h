#ifndef OBJ_H
#define OBJ_H

#include <GL/glew.h>
#include <glm.hpp>
#include <QList>

class Triangle;
// A simple parser that reads and writes Wavefront .obj files
class OBJ
{
public:
    struct Index
    {
        int vertex;
        int coord;
        int normal;

        Index() : vertex(-1), coord(-1), normal(-1) {}
        Index(int vertex, int coord, int normal) : vertex(vertex), coord(coord), normal(normal) {}
    };

    struct Tri
    {
        Index a, b, c;

        Tri() {}
        Tri(const Index &a, const Index &b, const Index &c) : a(a), b(b), c(c) {}
    };

    OBJ(GLuint shader);
    ~OBJ();

    QList<glm::vec3> vertices;
    QList<glm::vec2> coords;
    QList<glm::vec3> normals;
    QList<Tri> triangles;

    void draw(glm::mat4 trans) const;
    bool read(const QString &path, QList<Triangle *> *tris);
    bool write(const QString &path) const;

private:
    void createVBO();
    Index getIndex(const QString &str) const;
    void fillVertex(int *i, GLfloat *data, Index index);

    GLuint m_shader;
    GLuint m_vaoID;
    GLuint m_vboID;
    int m_numVerts;


//    bool inBounds(const Index &index) const;

};

#endif // OBJ_H
