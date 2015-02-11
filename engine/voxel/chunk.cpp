#include "chunk.h"

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

    m_blocks = new int[m_size];
    for (int i = 0; i < m_size; i++)
        m_blocks[i] = 0;
}

Chunk::~Chunk()
{
    delete m_blocks;
}

void Chunk::buildChunk(int *heightMap, int w, int h)
{
    assert(m_dim.x == w && m_dim.y == h);

    int numBlocks;
    int index = 0;
    int type = 1;
    for (int z = 0; z < h; z++)
    {
        for (int x = 0; x < w; x++)
        {
            int mapI = z*w + x;

            numBlocks = glm::clamp(heightMap[mapI], m_p.y, m_p.y + m_dim.y);
            for (int y = m_p.y; y < numBlocks; y++)
            {
                int sides = 0;
                // check posz
                if (z == h-1 || heightMap[mapI + w] <= y)
                    sides |= (1 << 13);
                if (x == w-1 || heightMap[mapI + 1] <= y)
                    sides |= (1 << 12);
                if (z == 0 || heightMap[mapI - w] <= y)
                    sides |= (1 << 11);
                if (x == 0 || heightMap[mapI -1] <= y)
                    sides |= (1 << 10);
                if (y == numBlocks-1)
                    sides |= (1 << 9);
                if (y == m_p.y)
                    sides |= (1 << 8);

                if (y < -25)
                    type = STONE;
                else if (y == numBlocks - 1)
                    type = GRASS;
                else
                    type = DIRT;

                updateBlock(index++, (sides | type));
            }
            index += m_dim.y - (numBlocks - m_p.y);
        }
    }
}

void Chunk::updateBlock(int index, int info)
{
    m_blocks[index] = info;
    if (info >> 8) // at least one size needs to be rendered
        m_drawables.insert(index);
    else
        m_drawables.remove(index);
}

void Chunk::addBlock(int x, int y, int z, int type)
{
    Point p = Point(x, y, z) - m_p;
    m_blocks[(getIndex(p.x, p.y, p.z, m_dim))] = type;

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
}


int Chunk::getNeighbor(Point block, Point dir)
{
    Point n = block + dir;
    if (n.x < 0 || n.y < 0 || n.z < 0 || n.x >= m_dim.x ||
            n.y >= m_dim.y || n.z >= m_dim.z)
        return 0;
    return m_blocks[(getIndex(n.x, n.y, n.z, m_dim))];
}


void Chunk::removeBlock(int x, int y, int z)
{
    int index = getIndex(m_p.x - x, m_p.y - y, m_p.z - z, m_dim);
    m_blocks[index] = 0;
}

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
    return glm::vec4(m_p.x, m_p.y, m_p.z, 0.f);
}

int* Chunk::getBlocks(int *size)
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
//    char b;
//    glm::mat4 trans = glm::mat4();
//    glm::vec4 p = glm::vec4(m_p.x, m_p.y, m_p.z, 0);
//    for (int i = 0; i < m_size; i++)
//    {
//        if ((b = m_blocks[i]))
//        {
//            switch(b)
//            {
//            case AIR:
//                break;
//            case DEFAULT:
////                g->setAtlasPos();
//                g->setColor(0, 0, 1, 1, 0);
//                break;
//            case GRASS:
//                g->setColor(0, 1, 0, 1, 0);
//                break;
//            case DIRT:
//                g->setColor(1, 0, 0, 1, 0);
//                break;
//            case STONE:
//                g->setColor(0, 0, 0, 1, 0);
//                break;
//            default:
//                break;
//            }
//            trans[3] = getCoords(i, m_dim) + p;
//            g->drawCube(trans);
//        }
//    }
}


int Chunk::getIndex(int x, int y, int z, Point dim)
{
    return z*dim.y*dim.x + x*dim.y + y;
}


//glm::vec4 Chunk::getCoords(int index, Point dim)
//{
//    return glm::vec4(((index / dim.y) % dim.x),
//                     (index % dim.y),
//                     (index / (dim.x*dim.y)),
//                     0.f);
//}
