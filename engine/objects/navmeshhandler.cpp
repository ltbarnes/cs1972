#include "navmeshhandler.h"

#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>

NavMeshHandler::NavMeshHandler()
{
    m_obj = NULL;
    m_mesh.clear();

    m_shader = 0;
    m_vaoID = 0;
    m_vboID = 0;
}


NavMeshHandler::~NavMeshHandler()
{
}


void NavMeshHandler::setObject(OBJ *obj)
{
    if (m_obj)
        delete m_obj;
    m_obj = obj;
    if (m_obj)
        m_shader = obj->getShader();
}

bool NavMeshHandler::hasObject()
{
    return m_obj != NULL;
}

QList<Triangle*> *NavMeshHandler::getTriangleList()
{
    return &m_mesh;
}

void NavMeshHandler::draw(glm::mat4 trans)
{
    m_obj->draw(trans);
}

void NavMeshHandler::drawLines(glm::mat4 trans)
{
    glBindVertexArray(m_vaoID);
    glUniformMatrix4fv(glGetUniformLocation(m_shader, "model"), 1, GL_FALSE, glm::value_ptr(trans));
    glDrawArrays(GL_LINES, 0, m_numVerts);
    glBindVertexArray(0);
}

void NavMeshHandler::createVBO()
{
    // delete old array and buffer
    if (m_vaoID)
        glDeleteVertexArrays(1, &m_vaoID);
    if (m_vboID)
        glDeleteBuffers(1, &m_vboID);

    // build new data array
    m_numVerts = m_mesh.size() * 6;
    int size = m_numVerts * 3;
    GLfloat vertexData[size];

    int index = 0;
    foreach (Triangle *t, m_mesh)
    {
        fillVertex(&index, vertexData, t->vertices[0]);
        fillVertex(&index, vertexData, t->vertices[1]);
        fillVertex(&index, vertexData, t->vertices[1]);
        fillVertex(&index, vertexData, t->vertices[2]);
        fillVertex(&index, vertexData, t->vertices[2]);
        fillVertex(&index, vertexData, t->vertices[0]);
    }

    // Initialize the vertex array object.
    glGenVertexArrays(1, &m_vaoID);
    glBindVertexArray(m_vaoID);

    // Initialize the vertex buffer object.
    glGenBuffers(1, &m_vboID);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboID);

    glBufferData(GL_ARRAY_BUFFER, 3 * m_numVerts * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);

    GLuint position = glGetAttribLocation(m_obj->getShader(), "position");

    glEnableVertexAttribArray(position);
    glVertexAttribPointer(
        position,
        3,                   // Num coordinates per position
        GL_FLOAT,            // Type
        GL_FALSE,            // Normalized
        sizeof(GLfloat) * 3, // Stride
        (void*) 0            // Array buffer offset
    );

    // Unbind buffers.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void NavMeshHandler::fillVertex(int *index, GLfloat *data, glm::vec3 v)
{
    data[(*index)++] = v.x;
    data[(*index)++] = v.y;
    data[(*index)++] = v.z;
}




