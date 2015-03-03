#include "createscreen.h"
#include "actioncamera.h"
#include "application.h"
#include "minecraftworld.h"
#include "mcchunkbuilder.h"
#include "voxelmanager.h"
#include "player.h"

#include <QTime>

#define GLM_FORCE_RADIANS
#include <glm/gtx/transform.hpp>

#include <iostream>
using namespace std;

CreateScreen::CreateScreen(Application *parent)
    : Screen(parent)
{
    m_cam = new ActionCamera();
    m_cb = new MCChunkBuilder(QTime::currentTime().msec());

    VoxelManager *vm = new VoxelManager(m_cam, m_parentApp->getShader(SPARSE), Point(7, 2, 7), Point(32, 32, 32), m_cb);

    m_world = new MinecraftWorld(m_cam, vm, true);
    m_pos = glm::vec3(.1f, std::min(m_cb->getHeightAt(0, 0) + 30, 95), .1f);

    this->setCamera(m_cam);

    m_parentApp->setUseCubeMap(false);
    m_movement = 0;

    m_moveSlow = false;
}

CreateScreen::~CreateScreen()
{
    delete m_world;
}


// update and render
void CreateScreen::onTick(float secs)
{
//    assert(0);
    secs = glm::min(secs, .03f);

    float moveAmt = 1.3f;
    if (m_moveSlow)
        moveAmt = .1f;
    glm::vec3 dir = glm::vec3();

    if (m_movement & 0b001000)
        dir.z += 1;
    if (m_movement & 0b000100)
        dir.x -= 1;
    if (m_movement & 0b000010)
        dir.z -= 1;
    if (m_movement & 0b000001)
        dir.x += 1;
    if (m_movement & 0b100000)
        dir.y += 1;
    if (m_movement & 0b010000)
        dir.y -= 1;

    glm::vec4 look = m_cam->getLook();

    glm::vec3 move = glm::normalize(glm::vec3(look.x, 0.f, look.z)) * dir.z;
    move += glm::normalize(glm::vec3(-look.z, 0.f, look.x)) * dir.x;
    move.y = dir.y;
    if (glm::length(move) > 0.00001f)
        move = glm::normalize(move) * moveAmt;

    m_pos += move;
    m_cam->setCenter(m_pos);

    m_world->onTick(secs);

}

void CreateScreen::onRender(Graphics *g)
{
    g->setGraphicsMode(SPARSE);
    g->setTint(1, 1, 1);
    g->setPlayer(glm::vec3(m_cam->getEye()), 0);

    m_world->onDraw(g);
}



// key events
void CreateScreen::onKeyPressed(QKeyEvent *e)
{
    switch (e->key())
    {
    case Qt::Key_W:
        m_movement |= 0b001000;
        break;
    case Qt::Key_A:
        m_movement |= 0b000100;
        break;
    case Qt::Key_S:
        m_movement |= 0b000010;
        break;
    case Qt::Key_D:
        m_movement |= 0b000001;
        break;
    case Qt::Key_E:
        m_movement |= 0b100000;
        break;
    case Qt::Key_Q:
        m_movement |= 0b010000;
        break;
    case Qt::Key_Shift:
        m_moveSlow = true;
        break;
    default:
        break;
    }
}

void CreateScreen::onKeyReleased(QKeyEvent *e)
{
    switch (e->key())
    {
    case Qt::Key_W:
        m_movement &= 0b110111;
        break;
    case Qt::Key_A:
        m_movement &= 0b111011;
        break;
    case Qt::Key_S:
        m_movement &= 0b111101;
        break;
    case Qt::Key_D:
        m_movement &= 0b111110;
        break;
    case Qt::Key_E:
        m_movement &= 0b011111;
        break;
    case Qt::Key_Q:
        m_movement &= 0b101111;
        break;
    case Qt::Key_Shift:
        m_moveSlow = false;
        break;
    case Qt::Key_Backspace:
    case Qt::Key_Delete:
        m_parentApp->popScreens(1);
        break;
    default:
        break;
    }
}

void CreateScreen::onMouseMoved(QMouseEvent *, float deltaX, float deltaY)
{
    m_cam->yaw(deltaX / 10.f);
    m_cam->pitch(deltaY / 10.f);
}

void CreateScreen::onMousePressed(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
        m_world->addBlock();
    else if (e->button() == Qt::RightButton)
        m_world->removeBlock();
}

// unused in game
void CreateScreen::onMouseReleased(QMouseEvent *) {}
void CreateScreen::onMouseDragged(QMouseEvent *, float, float) {}
void CreateScreen::onMouseWheel(QWheelEvent *) {}
