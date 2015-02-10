#include "minecraftworld.h"
#include "point3d.h"
#include <QSet>

const char AIR = 0;
const char DEFAULT = 1;
const char GRASS = 2;
const char DIRT = 3;
const char STONE = 4;

MinecraftWorld::MinecraftWorld()
{
    m_vm = new VoxelManager(Point(), Point(1, 1, 1), Point(32, 32, 32));
}


MinecraftWorld::~MinecraftWorld()
{
    delete m_vm;
}


void MinecraftWorld::onDraw(Graphics *g)
{
    g->setAtlas("terrain.png", glm::vec2(16.f));

    QList<Chunk*> chunks = m_vm->getChunks();

    glm::mat4 trans = glm::mat4();
    int b;
    int *blocks;
    QSet<int> drawables;
    int size;
    glm::vec4 p;
    Point dim;
    foreach (Chunk *chunk, chunks)
    {
        blocks = chunk->getBlocks(&size);
        drawables = chunk->getDrawables();
        p = chunk->getLocationV();
        dim = chunk->getDimension();
        foreach (int i, drawables)
        {
            if ((b = blocks[i]))
            {
                switch(b & 0b11111111)
                {
                case AIR:
                    break;
                case DEFAULT:
                    g->setAtlasPosition(3,0);
                    break;
                case GRASS:
                    g->setAtlasPosition(0,0);
                    break;
                case DIRT:
                    g->setAtlasPosition(2,0);
                    break;
                case STONE:
                    g->setAtlasPosition(1,0);
                    break;
                default:
                    break;
                }
                trans[3] = getCoords(i, dim) + p;
                g->drawFaceCube(trans, b);
            }
        }
    }
}

glm::vec4 MinecraftWorld::getCoords(int index, Point dim)
{
    return glm::vec4(((index / dim.y) % dim.x),
                     (index % dim.y),
                     (index / (dim.x*dim.y)),
                     0.f);
}




void MinecraftWorld::addBlock(int x, int y, int z, char type)
{
    m_vm->addBlock(x, y, z, type);
}

