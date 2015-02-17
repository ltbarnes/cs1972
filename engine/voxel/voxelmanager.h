#ifndef VOXELMANAGER_H
#define VOXELMANAGER_H

#include "chunk.h"
#include <QHash>

class ChunkBuilder;

class VoxelManager// : public Manager
{
public:
    VoxelManager(GLuint shader, Point center, Point dim, Point chunkSize, ChunkBuilder *cb);
    virtual ~VoxelManager();

    QList<Chunk*> getChunks();
    void onDraw(Graphics *g);

//    void buildChunk(Point p, int *heightMap, int w, int h);
    void addChunk(Point pnt);

//    void addBlock(float x, float y, float z, char type);

    inline int getIndex(int x, int y, int z);

private:
    QHash<Point, Chunk *> m_chunks;
    ChunkBuilder *m_chunkBuilder;

    Point m_chunkSize;
    Point m_center, m_dim;
    Point m_max, m_min;

    GLuint m_shader;
};

#endif // VOXELMANAGER_H
