#include "mcchunkbuilder.h"
#include "chunk.h"

#include <iostream>
using namespace std;


MCChunkBuilder::MCChunkBuilder(int seed)
    : ChunkBuilder(seed)
{

}


MCChunkBuilder::~MCChunkBuilder()
{
}


Chunk* MCChunkBuilder::getChunk(Point p, Point dim)
{
    int size = dim.x * dim.z;
    int *hm = new int[size];

    float sqrt2 = glm::sqrt(2.f);

    int index = 0;
    for (int k = 0; k < dim.z; k++)
    {
        for (int i = 0; i < dim.x; i++)
        {
            float x = ((p.x + i) * (p.x + i) + (p.z + k) * (p.z + k)) * .003f + m_seed;
            hm[index++] = (int)glm::round((glm::cos(x) * glm::cos(sqrt2 * x)) * 4.f);
        }
    }

    Chunk *chunk = new Chunk(p, dim);
    chunk->buildChunk(hm, dim.x, dim.z);
    delete[] hm;
    return chunk;
}

