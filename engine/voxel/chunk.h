#ifndef CHUNK_H
#define CHUNK_H

#include "graphics.h"
#include "point3d.h"
#include <QSet>

enum BlockType
{
    AIR, GRASS, DIRT, STONE
//   0,    1,     2,    3
};

class Chunk
{
public:
    Chunk(Point p, Point dim);
    virtual ~Chunk();

    void buildChunk(int *heightMap, int w, int h);
    void updateBlock(int index, int info);
    int *getBlocks(int *size);
    QSet<int> getDrawables();


    Point getLocation();
    glm::vec4 getLocationV();
    Point getDimension();
    glm::vec4 getDimensionV();

    void addBlock(int x, int y, int z, int type);
    void removeBlock(int x, int y, int z);

    void onTick(float secs);
    void onDraw(Graphics *g);

    inline static int getIndex(int x, int y, int z, Point dim);
    inline int getNeighbor(Point block, Point dir);
//    static glm::vec4 getCoords(int index, Point dim);

private:
    int *m_blocks;

    QSet<int> m_drawables;

    Point m_neighbors[6];

    Point m_p, m_dim;
    int m_size;
};

#endif // CHUNK_H
