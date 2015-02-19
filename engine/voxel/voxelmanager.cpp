#include "voxelmanager.h"
#include "chunkbuilder.h"
#include <QHash>
#include "movableentity.h"
#include "collisionshape.h"

#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
using namespace std;
#include <glm/ext.hpp>

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

Collision *VoxelManager::predictCollision(MovableEntity *me, float secs)
{
    QList<CollisionShape *> cshapes = me->getCollisionShapes();
    Collision *col = NULL;

    glm::vec3 distance = me->getVelocity() * secs;

    foreach(CollisionShape *cs, cshapes)
    {
        col = checkCollisionY(me->getPosition(), cs, distance);
        if (col)
        {
            col->e1 = me;
            glm::vec3 diff = glm::abs(col->mtv - distance);
            glm::vec3 vel = me->getVelocity();
            if (diff.x > 0.0000001f)
                vel.x = 0;
            if (diff.y > 0.0000001f)
                vel.y = 0;
            if (diff.z > 0.0000001f)
                vel.z = 0;
            me->setVelocity(vel);
        }
    }

    return col;
}

Collision *VoxelManager::checkCollisionY(glm::vec3 pos, CollisionShape *cs, glm::vec3 distance)
{
    Collision *col = new Collision();
    col->mtv = distance;
    col->impulse.x = -1.f;

//    if (glm::length2(p) > 0.00001f)
//        return NULL;

    float bump = 0.50001f;

    glm::vec3 dir = glm::vec3(
                (distance.x >= 0 ? 1 : -1),
                (distance.y >= 0 ? 1 : -1),
                (distance.z >= 0 ? 1 : -1));

    glm::vec3 dim = cs->getDim() * .5f;
//    glm::vec3 pos = cs->getPos();
    glm::vec3 dest = pos + glm::vec3(distance.x, 0, 0);

    glm::vec3 minBlocks = glm::round(pos - dim);
    glm::vec3 maxBlocks = glm::round(pos + dim);

    glm::vec3 nearBlocks = glm::round(pos + dim * dir);
    glm::vec3 farBlocks = glm::round(dest + dim * dir);

    // check X
    int far = (int) (farBlocks.x + dir.x);
    for (int x = (int) nearBlocks.x; x != far; x += (int) dir.x)
    {
        for (int y = (int) minBlocks.y; y <= maxBlocks.y; y++)
        {
            for (int z = (int) minBlocks.z; z <= maxBlocks.z; z++)
            {
                Point bp = Point(roundDown(x, m_chunkSize.x), roundDown(y, m_chunkSize.y), roundDown(z, m_chunkSize.z));
                Chunk *c = m_chunks.value(bp, NULL);

                if (c && c->getSingleBlock(x - bp.x, y - bp.y, z - bp.z))
                {
                    col->mtv.x = farBlocks.x - ((pos.x + dim.x * dir.x));
                    col->mtv.x += (col->mtv.x > 0.f ? -bump : bump);

                    goto ENDX;
                }
            }
        }
    }
    ENDX:
    {}
    pos.x += col->mtv.x;

    dest = pos + glm::vec3(0, 0, distance.z);

    minBlocks = glm::round(pos - dim);
    maxBlocks = glm::round(pos + dim);

    nearBlocks = glm::round(pos + dim * dir);
    farBlocks = glm::round(dest + dim * dir);

    // check Z
    far = (int) (farBlocks.z + dir.z);
    for (int z = (int) nearBlocks.z; z != far; z += (int) dir.z)
    {
        for (int x = (int) minBlocks.x; x <= maxBlocks.x; x++)
        {
            for (int y = (int) minBlocks.y; y <= maxBlocks.y; y++)
            {
                Point bp = Point(roundDown(x, m_chunkSize.x), roundDown(y, m_chunkSize.y), roundDown(z, m_chunkSize.z));
                Chunk *c = m_chunks.value(bp, NULL);

                if (c && c->getSingleBlock(x - bp.x, y - bp.y, z - bp.z))
                {
                    col->mtv.z = farBlocks.z - ((pos.z + dim.z * dir.z));
                    col->mtv.z += (col->mtv.z > 0.f ? -bump : bump);

                    goto ENDZ;
                }
            }
        }
    }
    ENDZ:
    {}
    pos.z += col->mtv.z;

    dest = pos + glm::vec3(0, distance.y, 0);

    minBlocks = glm::round(pos - dim);
    maxBlocks = glm::round(pos + dim);

    nearBlocks = glm::round(pos + dim * dir);
    farBlocks = glm::round(dest + dim * dir);

    // check Y
    far = (int) (farBlocks.y + dir.y);
    for (int y = (int) nearBlocks.y; y != far; y += (int) dir.y)
    {
        for (int x = (int) minBlocks.x; x <= maxBlocks.x; x++)
        {
            for (int z = (int) minBlocks.z; z <= maxBlocks.z; z++)
            {
                Point bp = Point(roundDown(x, m_chunkSize.x), roundDown(y, m_chunkSize.y), roundDown(z, m_chunkSize.z));
                Chunk *c = m_chunks.value(bp, NULL);

                if (c && c->getSingleBlock(x - bp.x, y - bp.y, z - bp.z))
                {
                    col->mtv.y = farBlocks.y - ((pos.y + dim.y * dir.y));
                    col->mtv.y += (col->mtv.y > 0.f ? -bump : bump);

                    col->impulse.x = 1.f;
                    goto ENDY;
                }
            }
        }
    }
    ENDY:
    {}

    return col;
}

int VoxelManager::roundDown(int num, int multiple)
{
//    if (multiple == 0)
//        return num;
    int r = abs(num) % multiple;
    if (r == 0)
        return num;
    if (num < 0)
        return -(abs(num) + multiple - r);
    return num - r;
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

