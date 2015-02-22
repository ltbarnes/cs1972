#ifndef MCCHUNKBUILDER_H
#define MCCHUNKBUILDER_H

#include "chunkbuilder.h"
#include "perlinnoise.h"
#include <glm/glm.hpp>

enum BlockType
{
    AIR, ORIGINAL, GRASS, DIRT, STONE, SNOW
//   0,     1,       2,    3,     4     5
};

class MCChunkBuilder : public ChunkBuilder
{
public:
    MCChunkBuilder(int seed);
    virtual ~MCChunkBuilder();

    virtual Chunk *getChunk(GLuint shader, Point p, Point dim);

private:
    void buildChunk(GLuint shader, Chunk *chunk, int *heightMap, Point dim);
    void addFaces(int *index, GLfloat *vertexData, glm::vec3 center, int sides, BlockType type);
    void addVertex(int *index, float *vertexData, glm::vec3 v, glm::vec2 tex);

    glm::vec3 cube[24];
    PerlinNoise *m_noise;

};

#endif // MCCHUNKBUILDER_H
