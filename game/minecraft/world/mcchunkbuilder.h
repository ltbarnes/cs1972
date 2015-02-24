#ifndef MCCHUNKBUILDER_H
#define MCCHUNKBUILDER_H

#include "chunkbuilder.h"
#include "perlinnoise.h"
#include <glm/glm.hpp>
#include "point3d.h"
#include <QList>

enum BlockType
{
    AIR, ORIGINAL, GRASS, DIRT, STONE, SNOW, CRYSTAL, COAL
//   0,     1,       2,    3,     4     5       6      7
};

class MCChunkBuilder : public ChunkBuilder
{
public:
    MCChunkBuilder(int seed);
    virtual ~MCChunkBuilder();

    virtual Chunk *getChunk(GLuint shader, Point p, Point dim);
    virtual void resetChunk(GLuint shader, Chunk *chunk, Point dim);

    int getHeightAt(int x, int z);
    float getFloatHeightAt(int x, int z);

private:
    void buildChunk(GLuint shader, Chunk *chunk, int *heightMap, Point dim);
    void addFaces(int *index, GLfloat *vertexData, glm::vec3 center, int sides, BlockType type);
    void addVertex(int *index, float *vertexData, glm::vec3 v, glm::vec2 tex);

    glm::vec3 cube[24];
    PerlinNoise *m_noise;
    PerlinNoise *m_terrain;

};

#endif // MCCHUNKBUILDER_H
