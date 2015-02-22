#include "chunk.h"
#include "movableentity.h"
#include "collisionshape.h"

#include <iostream>
using namespace std;
#include <glm/ext.hpp>

Chunk::Chunk(Point p, Point dim)
{
    m_p = p;
    m_dim = dim;
    m_size = m_dim.x * m_dim.y * m_dim.z;

    m_neighbors[0] = Point( 0, 1, 0);
    m_neighbors[1] = Point( 1, 0, 0);
    m_neighbors[2] = Point( 0, 0, 1);
    m_neighbors[3] = Point(-1, 0, 0);
    m_neighbors[4] = Point( 0, 0,-1);
    m_neighbors[5] = Point( 0,-1, 0);

    m_blocks = 0;
    m_vaoID = 0;
    m_vboID = 0;
    m_blocks = new char[m_size];
    for (int i = 0; i < m_size; i++)
        m_blocks[i] = 0;
}

Chunk::~Chunk()
{
    if (m_blocks)
        delete[] m_blocks;

    if (m_vaoID)
        glDeleteVertexArrays(1, &m_vaoID);
    if (m_vboID)
        glDeleteBuffers(1, &m_vboID);
}

void Chunk::init(GLuint shader, char *blocks, QSet<int> drawables, float *vertexData, int numVerts)
{
    m_blocks = blocks;
    m_drawables = QSet<int>(drawables);
    setVBO(shader, vertexData, numVerts);
}


void Chunk::setVBO(GLuint shader, float *vertexData, int numVerts)
{
    m_numVerts = numVerts;

    if (m_vaoID)
        glDeleteVertexArrays(1, &m_vaoID);
    if (m_vboID)
        glDeleteBuffers(1, &m_vboID);

    // Initialize the vertex array object.
    glGenVertexArrays(1, &m_vaoID);
    glBindVertexArray(m_vaoID);

    // Initialize the vertex buffer object.
    glGenBuffers(1, &m_vboID);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboID);

    glBufferData(GL_ARRAY_BUFFER, 5 * m_numVerts * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);

    GLuint position = glGetAttribLocation(shader, "position");
    GLuint texCoord = glGetAttribLocation(shader, "texCoord");

    glEnableVertexAttribArray(position);
    glVertexAttribPointer(
        position,
        3,                   // Num coordinates per position
        GL_FLOAT,            // Type
        GL_FALSE,            // Normalized
        sizeof(GLfloat) * 5, // Stride
        (void*) 0            // Array buffer offset
    );
    glEnableVertexAttribArray(texCoord);
    glVertexAttribPointer(
        texCoord,
        2,                              // Num coordinates per position
        GL_FLOAT,                       // Type
        GL_TRUE,                        // Normalized
        sizeof(GLfloat) * 5,            // Stride
        (void*) (sizeof(GLfloat) * 3)   // Array buffer offset
    );

    // Unbind buffers.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Chunk::updateBlock(int index, int drawable, char type)
{
    m_blocks[index] = type;
    if (drawable) // at least one size needs to be rendered
        m_drawables.insert(index);
    else
        m_drawables.remove(index);
}

void Chunk::addBlock(Point p)
{
    m_drawables.insert(getIndex(p.x, p.y, p.z, m_dim));
}


int Chunk::getNeighbor(Point block, Point dir)
{
    Point n = block + dir;
    if (n.x < 0 || n.y < 0 || n.z < 0 || n.x >= m_dim.x ||
            n.y >= m_dim.y || n.z >= m_dim.z)
        return 0;
    return m_blocks[(getIndex(n.x, n.y, n.z, m_dim))];
}


//void Chunk::removeBlock(int x, int y, int z)
//{
//    int index = getIndex(m_p.x - x, m_p.y - y, m_p.z - z, m_dim);
//    m_blocks[index] = 0;

//     // check neighbors
//}

void Chunk::onTick(float) {}

Point Chunk::getLocation()
{
    return m_p;
}

glm::vec4 Chunk::getLocationV()
{
    return glm::vec4(m_p.x, m_p.y, m_p.z, 1.f);
}

Point Chunk::getDimension()
{
    return m_dim;
}

glm::vec4 Chunk::getDimensionV()
{
    return glm::vec4(m_dim.x, m_dim.y, m_dim.z, 0.f);
}

char* Chunk::getBlocks(int *size)
{
    *size = m_size;
    return m_blocks;
}


QSet<int> Chunk::getDrawables()
{
    return m_drawables;
}

void Chunk::onDraw(Graphics *g)
{
//    g->setColor(1, 1, 1, 1, 0);
    glBindVertexArray(m_vaoID);
    glDrawArrays(GL_TRIANGLES, 0, m_numVerts);
    glBindVertexArray(0);
}


char Chunk::getSingleBlock(int x, int y, int z)
{
    assert( x >= 0 && x < m_dim.x);
    if( y < 0 || y >= m_dim.y)
            cout << "y: " << y << endl;
    assert( y >= 0 && y < m_dim.y);
    assert( z >= 0 && z < m_dim.z);

    return m_blocks[getIndex(x, y, z, m_dim)];
}


int Chunk::getIndex(int x, int y, int z, Point dim)
{
    return z*dim.y*dim.x + x*dim.y + y;
}


