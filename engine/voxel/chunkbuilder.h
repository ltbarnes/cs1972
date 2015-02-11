#ifndef CHUNKBUILDER_H
#define CHUNKBUILDER_H

struct Point;
class Chunk;

class ChunkBuilder
{
public:
    ChunkBuilder(int seed) { m_seed = seed; }
    virtual ~ChunkBuilder() {}

    virtual Chunk* getChunk(Point p, Point dim) = 0;

protected:
    int m_seed;

};

#endif // CHUNKBUILDER_H
