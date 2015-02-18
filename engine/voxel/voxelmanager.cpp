#include "voxelmanager.h"
#include "chunkbuilder.h"
#include <QHash>

#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
using namespace std;

VoxelManager::VoxelManager(GLuint shader, Point center, Point dim, Point chunkSize, ChunkBuilder *cb)
{
    m_center = center;
    m_dim = dim;
    m_chunkSize = chunkSize;
    m_shader = shader;

    m_chunkBuilder = cb;

    m_min = Point(0, 0, 0);
    m_max = Point(0, 0, 0);

    Point p;
    for (int x = -m_dim.x; x < m_dim.x; x++)
    {
        for (int y = -m_dim.y; y < m_dim.y; y++)
        {
            for (int z = -m_dim.z; z < m_dim.z; z++)
            {
                p = Point(x, y, z) * m_chunkSize;
                addChunk(p);
            }
        }
    }
//    addChunk(Point(-32, -32, -32));
}

VoxelManager::~VoxelManager()
{
    // world deletes them
//    foreach(Chunk *chunk, m_chunks)
//        delete chunk;

    delete m_chunkBuilder;
}


void VoxelManager::addChunk(Point pnt)
{
    Chunk *chunk = m_chunkBuilder->getChunk(m_shader, pnt, m_chunkSize);

    Point p = chunk->getLocation();
    m_chunks.insert(p, chunk);

}


QList<Chunk*> VoxelManager::getChunks()
{
    return m_chunks.values();
}


void VoxelManager::onDraw(Graphics *g)
{
    glm::mat4 frust = g->getFrustum();

    glm::vec4 planes[6];
    planes[0] = frust[3] - frust[0]; // negx
    planes[1] = frust[3] - frust[1]; // negy
    planes[2] = frust[3] - frust[2]; // negz
    planes[3] = frust[3] + frust[0]; // posx
    planes[4] = frust[3] + frust[1]; // posy
    planes[5] = frust[3] + frust[2]; // posz


    glm::mat4 trans = glm::mat4();

    glm::vec4 pos, dim;
    glm::vec4 plan, corn;
    glm::vec4 corners[8];

    foreach (Chunk *c, m_chunks)
    {
        pos = c->getLocationV();
        dim = c->getDimensionV();

        corners[0] = pos + dim * glm::vec4(-1,-1,-1, 0 );
        corners[1] = pos + dim * glm::vec4( 1,-1,-1, 0 );
        corners[2] = pos + dim * glm::vec4(-1, 1,-1, 0 );
        corners[3] = pos + dim * glm::vec4(-1,-1, 1, 0 );
        corners[4] = pos + dim * glm::vec4(-1, 1, 1, 0 );
        corners[5] = pos + dim * glm::vec4( 1,-1, 1, 0 );
        corners[6] = pos + dim * glm::vec4( 1, 1,-1, 0 );
        corners[7] = pos + dim * glm::vec4( 1, 1, 1, 0 );

        int numOutside;
        bool render = true;
        for (int p = 0; p < 6; p++)
        {
            plan = planes[p];
            numOutside = 0;

            for (int c = 0; c < 8; c++)
            {
                corn = corners[c];

                // inside plane
                if (glm::dot(plan, corn) > 0.f)
                {
                    break;
                }
                numOutside++;
            }
            if (numOutside >= 8)
            {
                render = false;
                break;
            }
        }

        if (render)
        {
            trans[3] = pos;
            glUniformMatrix4fv(glGetUniformLocation(m_shader, "model"), 1, GL_FALSE, glm::value_ptr(trans));
            c->onDraw(g);
        }
    }
}

//void VoxelManager::addBlock(float x, float y, float z, char type)
//{
//    // add single block to chunk
//    Point p;
//    p.x = (x >= 0 ? x / m_chunkSize.x : x / m_chunkSize.x - m_chunkSize.x);
//    p.y = (y >= 0 ? y / m_chunkSize.y : y / m_chunkSize.y - m_chunkSize.y);
//    p.z = (z >= 0 ? z / m_chunkSize.z : z / m_chunkSize.z - m_chunkSize.z);

//    Chunk *c = m_chunks.value(p);
//    assert(c);
//    c->addBlock(x, y, z, type);
//}


int VoxelManager::getIndex(int x, int y, int z)
{
    return z*m_chunkSize.y*m_chunkSize.x + x*m_chunkSize.y + y;
}

