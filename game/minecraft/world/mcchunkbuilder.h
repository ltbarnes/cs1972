#ifndef MCCHUNKBUILDER_H
#define MCCHUNKBUILDER_H

#include "chunkbuilder.h"

class MCChunkBuilder : public ChunkBuilder
{
public:
    MCChunkBuilder(int seed);
    virtual ~MCChunkBuilder();

    virtual Chunk* getChunk(Point p);
};

#endif // MCCHUNKBUILDER_H
