#include "mcchunkbuilder.h"
#include "perlinnoise.h"
#include "chunk.h"

//#include <iostream>
//using namespace std;
//#include <glm/ext.hpp>


MCChunkBuilder::MCChunkBuilder(int seed)
    : ChunkBuilder(seed)
{
    float rad = 0.5f;
    glm::vec3 tlf = glm::vec3(-rad, rad, rad);
    glm::vec3 trf = glm::vec3( rad, rad, rad);
    glm::vec3 blf = glm::vec3(-rad,-rad, rad);
    glm::vec3 brf = glm::vec3( rad,-rad, rad);

    glm::vec3 tlb = glm::vec3(-rad, rad, -rad);
    glm::vec3 trb = glm::vec3( rad, rad, -rad);
    glm::vec3 blb = glm::vec3(-rad,-rad, -rad);
    glm::vec3 brb = glm::vec3( rad,-rad, -rad);

    // front
    cube[0] = trf; cube[1] = tlf; cube[2] = brf; cube[3] = blf;
    // rightside
    cube[4] = trb; cube[5] = trf; cube[6] = brb; cube[7] = brf;
    // back
    cube[8] = tlb; cube[9] = trb; cube[10] = blb; cube[11] = brb;
    // leftside
    cube[12] = tlf; cube[13] = tlb; cube[14] = blf; cube[15] = blb;
    // top
    cube[16] = trb; cube[17] = tlb; cube[18] = trf; cube[19] = tlf;
    // bottom
    cube[20] = brf; cube[21] = blf; cube[22] = brb; cube[23] = blb;

    m_noise = new PerlinNoise(1.f, 2.f, 30.f, 5, m_seed);

    m_tallest = Point(0, std::numeric_limits<int>::min(), 0);
    m_smallest = Point(0, std::numeric_limits<int>::max(), 0);
}


MCChunkBuilder::~MCChunkBuilder()
{
}


Point MCChunkBuilder::getTallest()
{
    return m_tallest;
}


Point MCChunkBuilder::getLowest()
{
    return m_smallest;
}


int MCChunkBuilder::getHeightAt(int x, int z)
{
    return (int) glm::round(m_noise->GetHeight(x / 1000.0, z / 1000.0));
}


Chunk *MCChunkBuilder::getChunk(GLuint shader, Point p, Point dim)
{
    int size = dim.x * dim.z;
    int *hm = new int[size];

    int index = 0;
    Point point;
    for (int k = 0; k < dim.z; k++)
    {
        for (int i = 0; i < dim.x; i++)
        {
            point = Point(p.x + i, 0, p.z + k);
            point.y = (int) glm::round(m_noise->GetHeight(point.x / 1000.0, point.z / 1000.0));
            hm[index++] = point.y;
            if ((point.x * point.x + point.z * point.z) < 50000 && point.y > m_tallest.y)
                m_tallest = point;
            if (point.y < m_smallest.y)
                m_smallest = point;
        }
    }

    Chunk *chunk = new Chunk(p, dim);
    buildChunk(shader, chunk, hm, dim);
    delete[] hm;
    return chunk;
}

void MCChunkBuilder::buildChunk(GLuint shader, Chunk *chunk, int *heightMap, Point dim)
{
    QSet<int> drawables;
    char *blocks = new char[dim.x * dim.y * dim.z];
    int w = dim.x;
    int h = dim.z;

    Point p = chunk->getLocation();

    // 6 per face * 6 faces per block * num total possible blocks
    int numVerts = 36 * dim.x * dim.y * dim.z;
    int size = numVerts * 5; // three points and two tex coords
    GLfloat *vertexData = new GLfloat[size];

    int numBlocks;
    int index = 0;
    numVerts = 0;
    BlockType type = ORIGINAL;
    for (int z = 0; z < h; z++)
    {
        for (int x = 0; x < w; x++)
        {
            int mapI = z*w + x;

            numBlocks = glm::clamp(heightMap[mapI], p.y, p.y + dim.y);
            for (int y = p.y; y < numBlocks; y++)
            {
                char sides = 0;
                // check posz
                if (z == h-1 || heightMap[mapI + w] <= y)
                    sides |= (1 << 5);
                if (x == w-1 || heightMap[mapI + 1] <= y)
                    sides |= (1 << 4);
                if (z == 0 || heightMap[mapI - w] <= y)
                    sides |= (1 << 3);
                if (x == 0 || heightMap[mapI -1] <= y)
                    sides |= (1 << 2);
                if (y == numBlocks-1)
                    sides |= (1 << 1);
                if (y == p.y)
                    sides |= (1);

                if (y < -25)
                    type = STONE;
                else if (y == heightMap[mapI] - 1)
                    type = SNOW;
                else
                    type = DIRT;

                if (sides)
                {
                    drawables.insert(index);
                    addFaces(&numVerts, vertexData, glm::vec3(x, y-p.y, z), sides, type);
                }

                blocks[index++] = type;
            }
            int negSpace = dim.y - (numBlocks - p.y);
            for (int y = 0; y < negSpace; y++)
                blocks[index++] = 0;
        }
    }
    chunk->init(shader, blocks, drawables, vertexData, numVerts / 5);
}

void MCChunkBuilder::addFaces(int* index, GLfloat *vertexData, glm::vec3 center, int sides, BlockType type)
{
    float subTex = 1.f / 16.f;
    glm::vec2 uv = glm::vec2();

    for (int i = 0; i < 6; i++)
    {
        if (type == ORIGINAL)
        {
            if (sides & 0b10 && i == 4)
                uv = glm::vec2(0, 0);
            else if (sides * 0b1 && i == 5)
                uv = glm::vec2(2, 0);
            else if (sides & (1 << (5 - i)))
                uv = glm::vec2(3, 0);
        }
        else if (type == SNOW)
        {
            if (sides & 0b10 && i == 4)
                uv = glm::vec2(2, 4);
            else if (sides * 0b1 && i == 5)
                uv = glm::vec2(2, 0);
            else if (sides & (1 << (5 - i)))
                uv = glm::vec2(4, 4);

        }
        else if (type == GRASS)
            uv = glm::vec2(0, 0);
        else if(type == DIRT)
            uv = glm::vec2(2, 0);
        else if(type == STONE)
            uv = glm::vec2(1, 0);

        if (sides & (1 << (5 - i)))
        {
            addVertex(index, vertexData, cube[i*4 + 0] + center, (uv + glm::vec2(1, 0)) * subTex );
            addVertex(index, vertexData, cube[i*4 + 1] + center, (uv + glm::vec2(0)) * subTex );
            addVertex(index, vertexData, cube[i*4 + 2] + center, (uv + glm::vec2(1)) * subTex );
            addVertex(index, vertexData, cube[i*4 + 1] + center, (uv + glm::vec2(0)) * subTex );
            addVertex(index, vertexData, cube[i*4 + 3] + center, (uv + glm::vec2(0, 1)) * subTex );
            addVertex(index, vertexData, cube[i*4 + 2] + center, (uv + glm::vec2(1)) * subTex );
        }
    }
}


void MCChunkBuilder::addVertex(int *index, float *vertexData, glm::vec3 v, glm::vec2 tex)
{
    vertexData[(*index)++] = v.x;
    vertexData[(*index)++] = v.y;
    vertexData[(*index)++] = v.z;
    vertexData[(*index)++] = tex.x;
    vertexData[(*index)++] = tex.y;
}


void MCChunkBuilder::resetChunk(GLuint shader, Chunk *chunk)
{

}
