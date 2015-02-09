#ifndef CHUNK_H
#define CHUNK_H

#include "graphics.h"
#include "point3d.h"

class Chunk
{
public:
    Chunk(Point p, Point dim);
    virtual ~Chunk();

    void buildChunk(int *heightMap, int w, int h);

    void addBlock(int x, int y, int z, char type);
    void removeBlock(int x, int y, int z);

    void onTick(float secs);
    void onDraw(Graphics *g);

    inline static int getIndex(int x, int y, int z, Point dim);
    inline static glm::vec4 getCoords(int index, Point dim);

private:
    char *m_blocks;

    Point m_p, m_dim;
    int m_size;
};

#endif // CHUNK_H
