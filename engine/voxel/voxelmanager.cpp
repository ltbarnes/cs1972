#include "voxelmanager.h"
#include "chunkbuilder.h"
#include <QHash>

#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
using namespace std;

VoxelManager::VoxelManager(GLuint shader, Point center, Point dim, Point chunkSize, ChunkBuilder *cb)
{
    m_center = center;
    m_dim = dim;
    m_chunkSize = chunkSize;
    m_shader = shader;

    m_chunkBuilder = cb;

    m_min = Point(0, 0, 0);
    m_max = Point(0, 0, 0);

    Point p;
    for (int x = -m_dim.x; x < m_dim.x; x++)
    {
        for (int y = -m_dim.y; y < m_dim.y; y++)
        {
            for (int z = -m_dim.z; z < m_dim.z; z++)
            {
                p = Point(x, y, z) * m_chunkSize;
                addChunk(p);
            }
        }
    }
//    addChunk(Point(-32, -32, -32));
}

VoxelManager::~VoxelManager()
{
    // world deletes them
//    foreach(Chunk *chunk, m_chunks)
//        delete chunk;

    delete m_chunkBuilder;
}


void VoxelManager::addChunk(Point pnt)
{
    Chunk *chunk = m_chunkBuilder->getChunk(m_shader, pnt, m_chunkSize);

    Point p = chunk->getLocation();
    m_chunks.insert(p, chunk);

}


QList<Chunk*> VoxelManager::getChunks()
{
    return m_chunks.values();
}


void VoxelManager::onDraw(Graphics *g)
{
    glm::mat4 trans = glm::mat4();
    foreach (Chunk *c, m_chunks) {
        trans[3] = c->getLocationV();
        glUniformMatrix4fv(glGetUniformLocation(m_shader, "model"), 1, GL_FALSE, glm::value_ptr(trans));
        c->onDraw(g);
    }
}

//void VoxelManager::addBlock(float x, float y, float z, char type)
//{
//    // add single block to chunk
//    Point p;
//    p.x = (x >= 0 ? x / m_chunkSize.x : x / m_chunkSize.x - m_chunkSize.x);
//    p.y = (y >= 0 ? y / m_chunkSize.y : y / m_chunkSize.y - m_chunkSize.y);
//    p.z = (z >= 0 ? z / m_chunkSize.z : z / m_chunkSize.z - m_chunkSize.z);

//    Chunk *c = m_chunks.value(p);
//    assert(c);
//    c->addBlock(x, y, z, type);
//}


int VoxelManager::getIndex(int x, int y, int z)
{
    return z*m_chunkSize.y*m_chunkSize.x + x*m_chunkSize.y + y;
}

