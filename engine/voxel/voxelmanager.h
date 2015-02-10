#ifndef VOXELMANAGER_H
#define VOXELMANAGER_H

#include "chunk.h"
//#include "point3d.h"
#include <QHash>

class VoxelManager// : public Manager
{
public:
    VoxelManager(Point center, Point dim, Point chunkSize);
    virtual ~VoxelManager();

    QList<Chunk*> getChunks();
    void onDraw(Graphics *g);

    void buildChunk(Point p, int *heightMap, int w, int h);
    void addChunk(Chunk * chunk);

    void addBlock(float x, float y, float z, char type);

    inline int getIndex(int x, int y, int z);

private:
    QHash<Point, Chunk *> m_chunks;

    Point m_chunkSize;
    Point m_center, m_dim;
    Point m_max, m_min;
};

#endif // VOXELMANAGER_H
