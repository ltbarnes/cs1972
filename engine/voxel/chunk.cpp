#include "chunk.h"

const char AIR = 0;
const char DEFAULT = 1;
const char GRASS = 2;
const char DIRT = 3;
const char STONE = 4;

#include <iostream>
using namespace std;
//#include <glm/ext.hpp>

Chunk::Chunk(Point p, Point dim)
{
    m_p = p;
    m_dim = dim;
    m_size = m_dim.x * m_dim.y * m_dim.z;

    m_blocks = new char[m_size];
    for (int i = 0; i < m_size; i++)
        m_blocks[i] = 0;

    int hm[35] = {1, 2, 3, 4, 5,
                  2, 3, 4, 5, 6,
                  3, 4, 5, 6, 7,
                  4, 4, 5, 6, 6,
                  5, 5, 5, 5, 5,
                  6, 5, 5, 5, 4,
                  6, 5, 4, 4, 3};

    if (p == Point())
        buildChunk(hm, 5, 7);
}

Chunk::~Chunk()
{
    delete m_blocks;
}

void Chunk::buildChunk(int *heightMap, int w, int h)
{
    int numBlocks;
    for (int z = 0; z < h; z++)
    {
        for (int x = 0; x < w; x++)
        {
            numBlocks = heightMap[(z*w + x)];
            for (int y = 0; y < numBlocks; y++)
                addBlock(x, y, z, 1);
        }
    }
}

void Chunk::addBlock(int x, int y, int z, char type)
{
    m_blocks[(getIndex(x - m_p.x, y - m_p.y, z - m_p.z, m_dim))] = type;
}


void Chunk::removeBlock(int x, int y, int z)
{
    int index = getIndex(m_p.x - x, m_p.y - y, m_p.z - z, m_dim);
    m_blocks[index] = 0;
}

void Chunk::onTick(float) {}


void Chunk::onDraw(Graphics *g)
{
    char b;
    glm::mat4 trans = glm::mat4();
    glm::vec4 p = glm::vec4(m_p.x, m_p.y, m_p.z, 0);
    for (int i = 0; i < m_size; i++)
    {
        if ((b = m_blocks[i]))
        {
            switch(b)
            {
            case AIR:
                break;
            case DEFAULT:
//                g->setAtlasPos();
                g->setColor(0, 0, 1, 1, 0);
                break;
            case GRASS:
                g->setColor(0, 1, 0, 1, 0);
                break;
            case DIRT:
                g->setColor(1, 0, 0, 1, 0);
                break;
            case STONE:
                g->setColor(0, 0, 0, 1, 0);
                break;
            default:
                break;
            }
            trans[3] = getCoords(i, m_dim) + p;
            g->drawCube(trans);
        }
    }
}


int Chunk::getIndex(int x, int y, int z, Point dim)
{
    return z*dim.y*dim.x + x*dim.y + y;
}


glm::vec4 Chunk::getCoords(int index, Point dim)
{
    int y = index % dim.y;
    int x = (index / dim.y) % dim.x;
    int z = index / (dim.x*dim.y);
    return glm::vec4(x, y, z, 1);
}
