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

//void Chunk::addBlock(int x, int y, int z, int type)
//{
//    Point p = Point(x, y, z) - m_p;
//    m_blocks[(getIndex(p.x, p.y, p.z, m_dim))] = type;

//    int b;
//    Point n;
//    for (int i = 0; i < 6; i++) {
//        n = m_neighbors[i] + p;
//        if ((b = m_blocks[(getIndex(n.x, n.y, n.z, m_dim))]))
//        {

//        }
//        else
//        {

//        }
//    }
//    m_drawables.
//}


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

void Chunk::onDraw(Graphics *)
{
    glBindVertexArray(m_vaoID);
    glDrawArrays(GL_TRIANGLES, 0, m_numVerts);
    glBindVertexArray(0);
}


int Chunk::getIndex(int x, int y, int z, Point dim)
{
    return z*dim.y*dim.x + x*dim.y + y;
}

//QList<Collision *> Chunk::collides(Entity *e, float secs)
//{
//    MovableEntity *me = dynamic_cast<MovableEntity* >(e);
//    QList<CollisionShape *> cshapes = me->getCollisionShapes();
//    QList<Collision *> cols;
//    Collision *col;

//    glm::vec3 distance = e->getVelocity() * secs;

//    foreach(CollisionShape *cs, cshapes)
//    {
//        col = checkCollisionY(cs, distance);
//        if (col)
//        {
//            col->e1 = this;
//            col->e2 = e;
//            cols.append(col);
//        }
//    }

//    return cols;
//}

//Collision *Chunk::checkCollisionY(CollisionShape *cs, glm::vec3 distance)
//{
//    Collision *col = NULL;
//    glm::vec3 mtv = glm::vec3();

//    glm::vec3 p = glm::vec3(m_p.x, m_p.y, m_p.z);

//    if (glm::length2(p) > 0.00001f)
//        return NULL;

//    glm::vec3 dir = glm::vec3(
//                (distance.x >= 0 ? 1 : -1),
//                (distance.y >= 0 ? 1 : -1),
//                (distance.z >= 0 ? 1 : -1));

//    glm::vec3 dim = cs->getDim() * .5f;
//    glm::vec3 pos = cs->getPos() - p;
//    glm::vec3 dest = pos + distance;

//    glm::vec3 minBlocks = glm::round(pos - dim);
//    glm::vec3 maxBlocks = glm::round(pos + dim);

//    glm::vec3 nearBlocks = glm::round(pos + dim * dir);
//    glm::vec3 farBlocks = glm::round(dest + dim * dir);

//    // check Y
//    if (contains(nearBlocks) || contains(farBlocks))
//    {
//        int near = (int) /*glm::clamp(*/nearBlocks.y/*, 0.f, m_dim.y * 1.f)*/;
//        int far = (int) /*glm::clamp(*/(farBlocks.y + dir.y)/*, 0.f, m_dim.y * 1.f)*/;
//        for (int y = near; y != far; y += (int) dir.y)
//        {
//            for (int x = (int) minBlocks.x; x <= maxBlocks.x; x++)
//            {
//                for (int z = (int) minBlocks.z; z <= maxBlocks.z; z++)
//                {
//                    if (m_blocks[getIndex(x, y, z, m_dim)])
//                    {
//                        mtv.y = farBlocks.y - ((pos.y + dim.y * dir.y));
//                        mtv.y += (mtv.y > 0.f ? -0.500001f : 0.500001f);
//                        cout << "COLLISION: " << mtv.y << endl;
//                        goto END;
//                    }
//                }
//            }
//        }
//    }
//    END:
////        cout << endl;

//        if (glm::length2(p) < 0.00001f)
//        {
//        cout << "BLOCK: " << glm::to_string(p) << endl;
//        cout << "dist: " << glm::to_string(distance) << endl;
//        cout << "dir: " << glm::to_string(dir) << endl;
//        cout << "pos: " << glm::to_string(pos) << endl;
//        cout << "dest: " << glm::to_string(dest) << endl;
//        cout << "min: " << glm::to_string(minBlocks) << endl;
//        cout << "max: " << glm::to_string(maxBlocks) << endl;
//        cout << "near: " << glm::to_string(nearBlocks) << endl;
//        cout << "far: " << glm::to_string(farBlocks) << endl;
//        cout << "block: " << (int) m_blocks[getIndex(26, 1, 26, m_dim)] << endl;
//        cout << "block: " << (int) m_blocks[getIndex(26, 0, 26, m_dim)] << endl << endl;
//        }

//        if (glm::length2(mtv) > 0.000001)
//        {
//            col = new Collision();
//            col->mtv = mtv;
//        }

//        return col;
//}

//void Chunk::handleCollision(Collision *) {}


//bool Chunk::contains(glm::vec3 point)
//{
////    glm::vec3 p = glm::vec3(m_p.x, m_p.y, m_p.z);
////    p = point - p;

//    glm::vec3 dim = glm::vec3(m_dim.x, m_dim.y, m_dim.z);

//    glm::bool3 less = glm::lessThan(point, dim);
//    glm::bool3 more = glm::greaterThanEqual(point, glm::vec3());

//    return less.x && less.y && less.z && more.x && more.y && more.z;
//}


//void Chunk::onDrawOpaque(Graphics *) {}
//void Chunk::onDrawTransparent(Graphics *) {}

