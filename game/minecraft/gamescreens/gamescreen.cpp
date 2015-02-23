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

GameScreen::GameScreen(Application *parent)
    : Screen(parent)
{
    ActionCamera *cam = new ActionCamera();
    m_cb = new MCChunkBuilder(QTime::currentTime().msec());

    VoxelManager *vm = new VoxelManager(cam, m_parentApp->getShader(SPARSE), Point(6, 2, 6), Point(32, 32, 32), m_cb);

    m_world = new MinecraftWorld(cam, vm);
    glm::vec3 playerPos = glm::vec3(.1f, m_cb->getHeightAt(0, 0) + 10, .1f);
    m_player = new Player(cam, playerPos, m_world);

    m_world->addMovableEntity(m_player);

    this->setCamera(cam);

    m_parentApp->setUseCubeMap(true);

    count = 0;
    totalSecs = 0;
}

GameScreen::~GameScreen()
{
    delete m_world;
}


// update and render
void GameScreen::onTick(float secs)
{
    secs = glm::min(secs, .03f);
//    totalSecs += secs;
//    count++;
//    if (count > 100)
//    {
//        cout << (totalSecs / count) << endl;
//        count = 0;
//        totalSecs = 0;
//    }

    m_world->onTick(secs);
    m_player->setCameraPos();
}

void GameScreen::onRender(Graphics *g)
{
    int mode = m_player->getMode();
    glm::vec3 pos = m_player->getPosition();
    glm::vec3 look = glm::vec3(m_camera->getLook());

    g->setGraphicsMode(SPARSE);
    g->setPlayer(pos, mode);
//    g->setTint(.175f, .075f, .075f); // dark
    g->setTint(0, .5, 0); // green

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

    Point t = m_cb->getTallest();
    glm::mat4 trans = glm::mat4();
    trans[3] = glm::vec4(t.x, t.y, t.z, 1.f);

    g->setColor(1, 0, 0, 1, 0);
    g->drawCube(trans);

    Point l = m_cb->getLowest();
    trans[3] = glm::vec4(l.x, l.y + 2, l.z, 1.f);

    g->drawCube(trans);
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
}

void GameScreen::onMouseMoved(QMouseEvent *e, float deltaX, float deltaY)
{
    m_player->onMouseMoved(e, deltaX, deltaY);
}

void GameScreen::onMousePressed(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
        m_world->addBlock();
    else if (e->button() == Qt::RightButton)
        m_world->removeBlock();
}


// unused in game
void GameScreen::onMouseReleased(QMouseEvent *) {}
void GameScreen::onMouseDragged(QMouseEvent *, float, float) {}
void GameScreen::onMouseWheel(QWheelEvent *) {}
