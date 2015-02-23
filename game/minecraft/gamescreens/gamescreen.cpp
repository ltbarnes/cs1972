#include "gamescreen.h"
#include "camera.h"
#include "application.h"
#include "minecraftworld.h"
#include "mcchunkbuilder.h"
#include "player.h"
#include <QTime>

#define GLM_FORCE_RADIANS
#include <glm/gtx/transform.hpp>

#include <iostream>
using namespace std;


#define MAX_ALLIES 1
#define MAX_ENEMIES 10


GameScreen::GameScreen(Application *parent)
    : Screen(parent)
{
    ActionCamera *cam = new ActionCamera();
    m_cb = new MCChunkBuilder(QTime::currentTime().msec());

    VoxelManager *vm = new VoxelManager(cam, m_parentApp->getShader(SPARSE), Point(6, 2, 6), Point(32, 32, 32), m_cb);

    m_world = new MinecraftWorld(cam, vm, false);
    glm::vec3 playerPos = glm::vec3(.1f, m_cb->getHeightAt(0, 0) + 70, .1f);
    m_player = new Player(cam, playerPos, m_world);

    m_world->addMovableEntity(m_player);

    this->setCamera(cam);

    m_parentApp->setUseCubeMap(true);

    m_safety = Point(std::numeric_limits<int>::min());
    m_crawling = -1;
    m_scanning = -1;

    m_neighbors[0] = Point( 0, 0, 1);
    m_neighbors[1] = Point( 1, 0, 1);
    m_neighbors[2] = Point( 1, 0, 0);
    m_neighbors[3] = Point( 1, 0,-1);
    m_neighbors[4] = Point( 0, 0,-1);
    m_neighbors[5] = Point(-1, 0,-1);
    m_neighbors[6] = Point(-1, 0, 0);
    m_neighbors[7] = Point(-1, 0, 1);

    m_allies.clear();
    m_enemies.clear();
}

GameScreen::~GameScreen()
{
    delete m_world;
}


// update and render
void GameScreen::onTick(float secs)
{
    secs = glm::min(secs, .05f);

    m_world->onTick(secs);
    m_player->setCameraPos();

    if (m_crawling >= 0)
    {
        if (m_crawling-- == 0)
        {
            Point old = m_safety;
            m_safety = climb(m_safety);
            if (old == m_safety)
            {
                if (m_allies.size() < 3)
                    m_scanning = std::max(0, (m_safety.y + 25) * 20);
            }
            else
                m_crawling = 5;
        }
    }

    if (m_scanning >= 0)
    {
        if (m_scanning % 5 == 0)
        {
            cout << "scanning" << endl;
            scan();
        }

        if (m_scanning-- == 0)
        {
            cout << "nothing" << endl;
            cout << m_allies.size() << endl;
        }
//        scan();
    }
}

void GameScreen::scan()
{
    int r = m_safety.y * 20;
    Point p;
    p.x = rand() % (r * 2) - r;
    p.z = rand() % (r * 2) - r;
    p = p + m_safety;

    Point old = Point(std::numeric_limits<int>::min());
    while (old != p)
    {
        old = p;
        p = descend(old);
    }
    if (p.y < -25)
    {
        Point diff;
        foreach (Point a, m_allies)
        {
            diff = p - a;
            if (diff.x*diff.x + diff.y*diff.y + diff.z*diff.z < 2500)
                return;
        }
        placeAlly(p);
        cout << "found one" << endl;
        if (m_allies.size() >= MAX_ALLIES)
        {
            m_scanning = -1;
            cout << "found 'em all" << endl;
        }
    }
}


void GameScreen::placeAlly(Point p)
{
    m_allies.append(p);

//    for (int i = 0; i < 1; i++)
//    {
//        int r = 30;
//        Point e;
//        e.x = rand() % (r * 2) - r;
//        e.z = rand() % (r * 2) - r;
//        e.y = m_cb->getHeightAt(e.x, e.z);
//        m_world->addMovableEntity(new Enemy(glm::vec3(p.x, p.y + 10, p.z)));
//    }
}


Point GameScreen::climb(Point p)
{
    float height = m_cb->getFloatHeightAt(p.x, p.z);
    float bestHeight = height;
    Point bestP = p;

    Point next;
    for (int i = 0; i < 8; i++)
    {
        next = p + m_neighbors[i];
        height = m_cb->getFloatHeightAt(next.x, next.z);
        if (height > bestHeight)
        {
            bestHeight = height;
            bestP = next;
            bestP.y = (int) glm::round(bestHeight);
        }
    }
    return bestP;
}

Point GameScreen::descend(Point p)
{
    float height = m_cb->getFloatHeightAt(p.x, p.z);
    float bestHeight = height;
    Point bestP = p;

    Point next;
    for (int i = 0; i < 8; i++)
    {
        next = p + m_neighbors[i];
        height = m_cb->getFloatHeightAt(next.x, next.z);
        if (height < bestHeight)
        {
            bestHeight = height;
            bestP = next;
            bestP.y = (int) glm::round(bestHeight);
        }
    }
    return bestP;
}

void GameScreen::onRender(Graphics *g)
{
    int mode = m_player->getMode();
    glm::vec3 pos = m_player->getPosition();
    glm::vec3 look = glm::vec3(m_camera->getLook());

    g->setGraphicsMode(SPARSE);
    g->setPlayer(pos, mode);
    g->setTint(.175f, .075f, .075f); // dark
//    g->setTint(0, .5, 0); // green

    if (mode == 0)
        g->resetParticles();
    else
    {
        float fuzziness = 50.f;
        if (mode == 1) {
            g->setParticleForce(glm::vec3(0, -22, 0));
        } else if (mode == 2) {
            fuzziness = 200.f;
            g->setParticleForce(glm::vec3(0, -45, 0));
        }

        look.y = 0;
        look = glm::normalize(look) * .503f;
        g->drawParticles(pos - look, fuzziness);
    }

    m_world->onDraw(g);

//    Point t = m_cb->getTallest();
    Point t = m_safety;
    glm::mat4 trans = glm::mat4();
    trans[3] = glm::vec4(t.x, t.y + 1, t.z, 1.f);

    g->setColor(0, 0, 1, 1, 64);
    g->drawSphere(trans);

    g->setColor(0, 1, 0, 1, 0);
    foreach(Point v, m_allies)
    {
        trans[3] = glm::vec4(v.x, v.y + 2, v.z, 1.f);

        g->drawCone(trans);
    }
}


// key events
void GameScreen::onKeyPressed(QKeyEvent *e)
{
    m_player->onKeyPressed(e);

    if (e->key() == Qt::Key_Backspace || e->key() == Qt::Key_Delete)
        m_parentApp->popScreens(1);
}

void GameScreen::onKeyReleased(QKeyEvent *e)
{
    m_player->onKeyReleased(e);

    if (e->key() == Qt::Key_E)
    {
        m_scanning = -1;
        glm::vec3 pos = glm::round(m_player->getPosition());
        m_safety = Point((int) pos.x, 0, (int) pos.z);
        m_crawling = 1;
    }

//    if (e->key() == Qt::Key_Q)
//    {
//        glm::vec3 pos = glm::round(m_player->getPosition());
//        Point victim = Point((int) pos.x, 0, (int) pos.z);
//        Point old = Point(std::numeric_limits<int>::min());
//        while (old != victim)
//        {
//            old = victim;
//            victim = descend(old);
//        }
//        m_victims.append(victim);
//    }
}

void GameScreen::onMouseMoved(QMouseEvent *e, float deltaX, float deltaY)
{
    m_player->onMouseMoved(e, deltaX, deltaY);
}

void GameScreen::onMousePressed(QMouseEvent *)
{
//    if (e->button() == Qt::LeftButton)
//        m_world->addBlock();
//    else if (e->button() == Qt::RightButton)
//        m_world->removeBlock();
}


// unused in game
void GameScreen::onMouseReleased(QMouseEvent *) {}
void GameScreen::onMouseDragged(QMouseEvent *, float, float) {}
void GameScreen::onMouseWheel(QWheelEvent *) {}
