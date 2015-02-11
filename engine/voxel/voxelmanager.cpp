#include "voxelmanager.h"
#include "chunkbuilder.h"
#include <QHash>

#include <iostream>
using namespace std;

VoxelManager::VoxelManager(Point center, Point dim, Point chunkSize, ChunkBuilder *cb)
{
    m_center = center;
    m_dim = dim;
    m_chunkSize = chunkSize;

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
}

VoxelManager::~VoxelManager()
{
    foreach(Chunk *chunk, m_chunks)
        delete chunk;

    delete m_chunkBuilder;
}


void VoxelManager::addChunk(Point pnt)
{
    Chunk *chunk = m_chunkBuilder->getChunk(pnt, m_chunkSize);
//    Chunk *chunk = new Chunk(pnt, m_chunkSize);

    Point p = chunk->getLocation();
    m_chunks.insert(p, chunk);

    // check neighbor chunks and optimize faces
    if (m_chunks.contains(p + Point(0, 0, m_chunkSize.z)))
    {
        Chunk *nc = m_chunks.value(p + Point(0, 0, m_chunkSize.z));

        int size;
        int *cb = chunk->getBlocks(&size);
        int *ncb = nc->getBlocks(&size);
        for (int x = 0; x < m_chunkSize.x; x++)
        {
            for (int y = 0; y < m_chunkSize.y; y++)
            {
                int index = getIndex(x, y, m_chunkSize.z - 1);
                int block = cb[index];
                int indexn = getIndex(x, y, 0);
                int blockn = ncb[indexn];
                if (block && blockn)
                {
                    block &=  0b01111111111111;
                    blockn &= 0b11011111111111;
                    chunk->updateBlock(index, block);
                    nc->updateBlock(indexn, blockn);
                }
            }
        }
    }
    if (m_chunks.contains(p + Point(0, 0, -m_chunkSize.z)))
    {
        Chunk *nc = m_chunks.value(p + Point(0, 0, -m_chunkSize.z));

        int size;
        int *cb = chunk->getBlocks(&size);
        int *ncb = nc->getBlocks(&size);
        for (int x = 0; x < m_chunkSize.x; x++)
        {
            for (int y = 0; y < m_chunkSize.y; y++)
            {
                int index = getIndex(x, y, 0);
                int block = cb[index];
                int indexn = getIndex(x, y, m_chunkSize.z - 1);
                int blockn = ncb[indexn];
                if (block && blockn)
                {
                    block &=  0b11011111111111;
                    blockn &= 0b01111111111111;
                    chunk->updateBlock(index, block);
                    nc->updateBlock(indexn, blockn);
                }
            }
        }
    }
    // check neighbor chunks and optimize faces
    if (m_chunks.contains(p + Point(m_chunkSize.x, 0, 0)))
    {
        Chunk *nc = m_chunks.value(p + Point(m_chunkSize.x, 0, 0));

        int size;
        int *cb = chunk->getBlocks(&size);
        int *ncb = nc->getBlocks(&size);
        for (int z = 0; z < m_chunkSize.z; z++)
        {
            for (int y = 0; y < m_chunkSize.y; y++)
            {
                int index = getIndex(m_chunkSize.x - 1, y, z);
                int block = cb[index];
                int indexn = getIndex(0, y, z);
                int blockn = ncb[indexn];
                if (block && blockn)
                {
                    block &=  0b10111111111111;
                    blockn &= 0b11101111111111;
                    chunk->updateBlock(index, block);
                    nc->updateBlock(indexn, blockn);
                }
            }
        }
    }
    if (m_chunks.contains(p + Point(-m_chunkSize.x, 0, 0)))
    {
        Chunk *nc = m_chunks.value(p + Point(-m_chunkSize.x, 0, 0));

        int size;
        int *cb = chunk->getBlocks(&size);
        int *ncb = nc->getBlocks(&size);
        for (int z = 0; z < m_chunkSize.z; z++)
        {
            for (int y = 0; y < m_chunkSize.y; y++)
            {
                int index = getIndex(0, y, z);
                int block = cb[index];
                int indexn = getIndex(m_chunkSize.x - 1, y, z);
                int blockn = ncb[indexn];
                if (block && blockn)
                {
                    block &=  0b11101111111111;
                    blockn &= 0b10111111111111;
                    chunk->updateBlock(index, block);
                    nc->updateBlock(indexn, blockn);
                }
            }
        }
    }
    // check neighbor chunks and optimize faces
    if (m_chunks.contains(p + Point(0, m_chunkSize.y, 0)))
    {
        Chunk *nc = m_chunks.value(p + Point(0, m_chunkSize.y, 0));

        int size;
        int *cb = chunk->getBlocks(&size);
        int *ncb = nc->getBlocks(&size);
        for (int z = 0; z < m_chunkSize.z; z++)
        {
            for (int x = 0; x < m_chunkSize.x; x++)
            {
                int index = getIndex(x, m_chunkSize.y - 1, z);
                int block = cb[index];
                int indexn = getIndex(x, 0, z);
                int blockn = ncb[indexn];
                if (block && blockn)
                {
                    block &=  0b11110111111111;
                    blockn &= 0b11111011111111;
                    chunk->updateBlock(index, block);
                    nc->updateBlock(indexn, blockn);
                }
            }
        }
    }
    if (m_chunks.contains(p + Point(0,-m_chunkSize.y, 0)))
    {
        Chunk *nc = m_chunks.value(p + Point(0,-m_chunkSize.y, 0));

        int size;
        int *cb = chunk->getBlocks(&size);
        int *ncb = nc->getBlocks(&size);
        for (int z = 0; z < m_chunkSize.z; z++)
        {
            for (int x = 0; x < m_chunkSize.x; x++)
            {
                int index = getIndex(x, 0, z);
                int block = cb[index];
                int indexn = getIndex(x, m_chunkSize.y - 1, z);
                int blockn = ncb[indexn];
                if (block && blockn)
                {
                    block &=  0b11111011111111;
                    blockn &= 0b11110111111111;
                    chunk->updateBlock(index, block);
                    nc->updateBlock(indexn, blockn);
                }
            }
        }
    }
}


QList<Chunk*> VoxelManager::getChunks()
{
    return m_chunks.values();
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


int VoxelManager::getIndex(int x, int y, int z)
{
    return z*m_chunkSize.y*m_chunkSize.x + x*m_chunkSize.y + y;
}

