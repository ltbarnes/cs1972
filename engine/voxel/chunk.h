#ifndef CHUNK_H
#define CHUNK_H

#include "graphics.h"
#include "point3d.h"
#include "staticentity.h"
#include <QSet>

class Chunk : public StaticEntity
{
public:
    Chunk(Point p, Point dim);
    virtual ~Chunk();

    void init(GLuint shader, char *blocks, QSet<int> drawables, float *vertexData, int numVerts);

    void buildChunk(int *heightMap, int w, int h);
    void updateBlock(int index, int drawable, char type);
    char *getBlocks(int *size);
    QSet<int> getDrawables();


    Point getLocation();
    glm::vec4 getLocationV();
    Point getDimension();
    glm::vec4 getDimensionV();

//    void addBlock(int x, int y, int z, int type);
//    void removeBlock(int x, int y, int z);

    void onTick(float secs);
    void onDraw(Graphics *g);

    inline static int getIndex(int x, int y, int z, Point dim);
    inline int getNeighbor(Point block, Point dir);

    virtual void handleCollision(Collision *col);

private:
    char *m_blocks;

    QSet<int> m_drawables;

    Point m_neighbors[6];

    Point m_p, m_dim;
    int m_size;

    GLuint m_vaoID;
    GLuint m_vboID;
    int m_numVerts;
};

#endif // CHUNK_H
