#include "voxelmanager.h"
#include <QHash>

#include <iostream>
using namespace std;

VoxelManager::VoxelManager(Point center, Point dim, Point chunkSize)
{
    m_center = center;
    m_dim = dim;
    m_chunkSize = chunkSize;

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
                m_chunks.insert(p, new Chunk(p, m_chunkSize));
            }
        }
    }

}

VoxelManager::~VoxelManager()
{
}


void VoxelManager::onDraw(Graphics *g)
{
    foreach (Chunk *c, m_chunks) {
        c->onDraw(g);
    }
}


void VoxelManager::buildChunk(Point p, int *heightMap, int w, int h)
{
    Chunk *chunk;
    chunk = new Chunk(p, m_chunkSize);
    assert(w == m_chunkSize.x && h == m_chunkSize.z);
    chunk->buildChunk(heightMap, w, h);
}

void VoxelManager::addBlock(float x, float y, float z, char type)
{
    // add single block to chunk
    Point p;
    p.x = (x >= 0 ? x / m_chunkSize.x : x / m_chunkSize.x - m_chunkSize.x);
    p.y = (y >= 0 ? y / m_chunkSize.y : y / m_chunkSize.y - m_chunkSize.y);
    p.z = (z >= 0 ? z / m_chunkSize.z : z / m_chunkSize.z - m_chunkSize.z);

    Chunk *c = m_chunks.value(p);
    assert(c);
    c->addBlock(x, y, z, type);
}

