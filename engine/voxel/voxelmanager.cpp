#include "voxelmanager.h"
#include "chunkbuilder.h"
#include <QHash>
#include "movableentity.h"
#include "collisionshape.h"
#include "world.h"

#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
using namespace std;
#include <glm/ext.hpp>

VoxelManager::VoxelManager(Camera *cam, GLuint shader, Point center, Point dim, Point chunkSize, ChunkBuilder *cb)
    : Manager(SPARSE)
{
    m_camera = cam;
    m_shader = shader;
    m_center = center;
    m_dim = dim;
    m_chunkSize = chunkSize;

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


void VoxelManager::setAtlas(QString atlasName)
{
    m_atlasName = atlasName;
}


void VoxelManager::onDraw(Graphics *g)
{
    g->setAtlas(m_atlasName);

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

void VoxelManager::manage(World *world, float onTickSecs)
{
    QList<Collision *> cols;
    QList<MovableEntity *> mes = world->getMovableEntities();

    Collision *col;
    foreach(MovableEntity *me, mes)
    {
        col = predictCollision(me, onTickSecs);
        if (col)
            cols.append(col);
    }

    foreach(Collision *col, cols)
        col->e1->handleCollision(col);

}

Collision *VoxelManager::predictCollision(MovableEntity *me, float secs)
{
    QList<CollisionShape *> cshapes = me->getCollisionShapes();
    Collision *col = NULL;

    glm::vec3 distance = me->getVelocity() * secs;

    foreach(CollisionShape *cs, cshapes)
    {
        col = checkCollision3D(cs, distance);
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

Collision *VoxelManager::checkCollision3D(CollisionShape *cs, glm::vec3 distance)
{
    Collision *col;
    col = new Collision();
    col->mtv = distance;
    col->impulse = glm::vec3(-1.f);

    glm::vec3 dir = glm::vec3(
                (distance.x >= 0 ? 1 : -1),
                (distance.y >= 0 ? 1 : -1),
                (distance.z >= 0 ? 1 : -1));

    glm::vec3 dim = cs->getDim() * .5f;
    glm::vec3 pos = cs->getPos();
    glm::vec3 dest;

    if (abs(distance.x) < abs(distance.z))
    {
        // check x
        dest= pos + glm::vec3(distance.x, 0, 0);
        checkCollision1D(col, pos, dim, dest, dir.x, 0, 1, 2);
        pos.x += col->mtv.x;

        // check z
        dest = pos + glm::vec3(0, 0, distance.z);
        checkCollision1D(col, pos, dim, dest, dir.z, 2, 0, 1);
        pos.z += col->mtv.z;
    }
    else
    {
        // check z
        dest = pos + glm::vec3(0, 0, distance.z);
        checkCollision1D(col, pos, dim, dest, dir.z, 2, 0, 1);
        pos.z += col->mtv.z;

        // check x
        dest= pos + glm::vec3(distance.x, 0, 0);
        checkCollision1D(col, pos, dim, dest, dir.x, 0, 1, 2);
        pos.x += col->mtv.x;
    }

    // check y
    dest = pos + glm::vec3(0, distance.y, 0);
    checkCollision1D(col, pos, dim, dest, dir.y, 1, 2, 0);

    return col;
}

void VoxelManager::checkCollision1D(Collision *col, glm::vec3 pos, glm::vec3 dim, glm::vec3 dest,
                           float dir, int outer, int mid, int inner)
{
    float bump = 0.50001f;

    glm::vec3 minBlocks = glm::round(pos - dim);
    glm::vec3 maxBlocks = glm::round(pos + dim);

    glm::vec3 nearBlocks = glm::round(pos + dim * dir);
    glm::vec3 farBlocks = glm::round(dest + dim * dir);

    // check X
    int far = (int) (farBlocks[outer] + dir);
    for (int i = (int) nearBlocks[outer]; i != far; i += (int) dir)
    {
        for (int j = (int) minBlocks[mid]; j <= maxBlocks[mid]; j++)
        {
            for (int k = (int) minBlocks[inner]; k <= maxBlocks[inner]; k++)
            {
                int x, y, z;
                switch(outer) { case 0: x=i; break; case 1: y=i; break; case 2: z=i; break; }
                switch(mid) { case 0: x=j; break; case 1: y=j; break; case 2: z=j; break; }
                switch(inner) { case 0: x=k; break; case 1: y=k; break; case 2: z=k; break; }

                Point bp = Point(roundDown(x, m_chunkSize.x), roundDown(y, m_chunkSize.y), roundDown(z, m_chunkSize.z));
                Chunk *c = m_chunks.value(bp, NULL);

                if (c && c->getSingleBlock(x - bp.x, y - bp.y, z - bp.z))
                {
                    col->mtv[outer] = farBlocks[outer] - ((pos[outer] + dim[outer] * dir));
                    col->mtv[outer] += (col->mtv[outer] > 0.f ? -bump : bump);

                    col->impulse[outer] = 1.f;
                    return;
                }
            }
        }
    }
}

glm::vec3 VoxelManager::castRay(glm::vec3 p, glm::vec3 dir, float &t, int &face)
{
    Point step = Point( (dir.x > 0 ? 1 : -1),
                        (dir.y > 0 ? 1 : -1),
                        (dir.z > 0 ? 1 : -1));

    glm::vec3 tDelta = glm::abs(glm::vec3(1.f / dir.x, 1.f / dir.y, 1.f / dir.z));
    Point point = Point((int) glm::round(p.x), (int) glm::round(p.y), (int) glm::round(p.z));

    Point next = point + step;
    glm::vec3 nextf = glm::vec3(next.x - .5f * step.x, next.y - .5f * step.y, next.z - .5f * step.z);
    glm::vec3 tMax = glm::abs(nextf - p) * tDelta;

    Point bp = Point(roundDown(point.x, m_chunkSize.x), roundDown(point.y, m_chunkSize.y), roundDown(point.z, m_chunkSize.z));
    Chunk *c = m_chunks.value(bp, NULL);

    while(c)
    {
        if (c->getSingleBlock(point.x - bp.x, point.y - bp.y, point.z - bp.z))
            break;
        if(tMax.x < tMax.y)
        {
            if(tMax.x < tMax.z) {
                point.x += step.x;
                tMax.x += tDelta.x;
                face = 0b001000;
            } else {
                point.z += step.z;
                tMax.z += tDelta.z;
                face = 0b100000;
            }
        } else {
            if(tMax.y < tMax.z) {
                point.y += step.y;
                tMax.y += tDelta.y;
                face = 0b000010;
            } else {
                point.z += step.z;
                tMax.z += tDelta.z;
                face = 0b100000;
            }
        }
        bp = Point(roundDown(point.x, m_chunkSize.x), roundDown(point.y, m_chunkSize.y), roundDown(point.z, m_chunkSize.z));
        c = m_chunks.value(bp, NULL);
    }

    t = 0.f;

    if (step.z > 0 && face == 0b100000)
        face = 0b010000;
    else if (step.x > 0 && face == 0b001000)
        face = 0b000100;
    else if (step.y > 0 && face == 0b000010)
        face = 0b000001;

    return glm::vec3(point.x , point.y, point.z);
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

