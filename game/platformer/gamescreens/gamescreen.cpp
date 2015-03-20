#include "gamescreen.h"
#include "application.h"
#include "actioncamera.h"
#include "player.h"
#include "staticentity.h"
#include "platformerplayer.h"
#include "geometriccollisionmanager.h"
#include "triangle.h"

//#include <glm/ext.hpp>

GameScreen::GameScreen(Application *parent, int level)
    : Screen(parent)
{

    m_oh = new ObjectHandler();
    m_nmh = new NavMeshHandler();
    GLuint shader = m_parentApp->getShader(DEFAULT);

    QList<Triangle *> tris;
    QList<Triangle *> *navTris = m_nmh->getTriangleList();

    switch (level)
    {
    case 2:
        m_levelTexture = "level_hard.png";
        m_level = m_oh->getObject(":/objects/level_hard.obj", shader, &tris);
        break;
    case 3:
        m_levelTexture = "level_island.png";
        m_level = m_oh->getObject(":/objects/level_island.obj", shader, &tris);
        m_nmh->setObject(m_oh->getObject(":/objects/level_island_navmesh.obj", shader, navTris));
        m_nmh->createVBO();
        break;
    default: // 1
        m_levelTexture = "level_easy.png";
        m_level = m_oh->getObject(":/objects/level_easy.obj", shader, &tris);
        m_nmh->setObject(m_oh->getObject(":/objects/level_easy_navmesh.obj", shader, navTris));
        m_nmh->createVBO();
        break;
    }

    cout << tris.size() << endl;

    m_world = new PlatformerWorld();
    m_world->addToMesh(tris);

    ActionCamera *cam = new ActionCamera();
    cam->setCenter(glm::vec3(0, 2, 0));

    glm::vec3 playerPos = glm::vec3(0, 2, 0);
    PlatformerPlayer *player = new PlatformerPlayer(cam, playerPos);

    GeometricCollisionManager *gcm = new GeometricCollisionManager();
    m_world->addManager(gcm);

    m_world->setPlayer(player);
    m_world->setGravity(glm::vec3(0, -10, 0));

    setCamera(cam);

    m_drawEllipsoid = false;
    m_drawPoint = false;
    m_mouseDown = false;
    m_drawNavMesh = false;

    m_ellipsoid = new Ellipsoid(glm::vec3(), glm::vec3(.25f, .5, .25f), "rayshape");


    m_test = new Triangle(glm::vec3(0,2,-5),glm::vec3(-1,1,-5),glm::vec3(1,1,-5));
    QList<Triangle*> triTest;
    triTest.append(m_test);

    shader = m_parentApp->getShader(RAY);
    m_mb = new MeshBuffer();
    m_mb->setBuffer(shader, tris);
}

GameScreen::~GameScreen()
{
    delete m_oh;
    delete m_nmh;
    delete m_world;
    delete m_ellipsoid;
    delete m_mb;
    delete m_test;
}

// update and render
void GameScreen::onTick(float secs  )
{
    m_world->onTick(secs);

    m_drawPoint = true;

    glm::vec3 p = glm::vec3(m_world->getPlayer()->getEyePos());
    glm::vec3 d = glm::vec3(m_camera->getLook());

    float t;
    Triangle *tri = m_world->intersectWorld(p, d, &t);

    if (m_mouseDown)
    {
        if (tri)
        {
            m_drawEllipsoid = true;
            glm::vec3 pos = p + d * t;
//            pos += tri->normal * m_ellipsoid->getDim();
            m_ellipsoid->setPosition(pos);
        }
    }

    else if (m_drawEllipsoid)
    {
        float t2 = m_ellipsoid->intersectRayWorldSpace(p, d);
        if (t2 < t)
            t = t2;
    }

    if (t < INFINITY)
        m_point = p + d * t;
    else
        m_drawPoint = false;
}

void GameScreen::onRender(Graphics *g)
{
//    g->setWorldColor(.1, .1, .1);
//    g->setColor(1, 1, 1, 1, 0);


//    Light light1;
//    light1.type = POINT;
//    light1.color = glm::vec3(.5f, 1, .5f);
//    light1.posDir = glm::vec3(0, 30, 0);
//    light1.id = 1;

//    g->addLight(light1);

//    m_world->onDraw(g);

//    g->setAllWhite(true);
//    g->setTexture(m_levelTexture);
//    m_level->draw(glm::mat4());
//    g->setAllWhite(false);

//    g->setTexture("");
//    g->setColor(.25f, .75f, 1, 1, 0);
//    glm::mat4 trans;
//    if (m_drawEllipsoid)
//    {
//        trans = glm::translate(glm::mat4(), m_ellipsoid->getPos()) *
//                glm::scale(glm::mat4(), m_ellipsoid->getDim() * 2.f);
//        g->drawSphere(trans);
//    }
//    if (m_drawPoint)
//    {
//        trans = glm::translate(glm::mat4(), m_point) *
//                glm::scale(glm::mat4(), glm::vec3(0.1f));
//        g->setColor(1, 0, 0, 1, 0);
//        g->drawSphere(trans);
//    }

//    if (m_drawNavMesh && m_nmh->hasObject())
//    {
//        g->setColor(0, 1, 1, .3f, 0);
//        trans = glm::translate(glm::mat4(), glm::vec3(0, .3f, 0));

//        g->setTransparentMode(true);
//        m_nmh->draw(trans);

//        g->setTransparentMode(false);
//        m_nmh->drawLines(trans);
//    }
//    g->setColor(0, 0, 0, 1, 0);
//    g->drawLineSeg(glm::vec3(5, 6, 2), glm::vec3(-5, 6, -2), .1f);

    g->setGraphicsMode(RAY);
//    g->rayAddTriangles(m_world->getMesh());
    glBindBuffer(GL_UNIFORM_BUFFER, m_mb->getUBO());
    g->rayDrawQuad();
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void GameScreen::onMouseMoved(QMouseEvent *e, float deltaX, float deltaY)
{
    m_world->onMouseMoved(e, deltaX, deltaY);
}
void GameScreen::onMousePressed(QMouseEvent *)
{
    m_mouseDown = true;
}
void GameScreen::onKeyReleased(QKeyEvent *e )
{
    if (e->key() == Qt::Key_N)
        m_drawNavMesh = !m_drawNavMesh;
    m_world->onKeyReleased(e);
}
void GameScreen::onMouseDragged(QMouseEvent *e, float deltaX, float deltaY)
{
    m_world->onMouseMoved(e, deltaX, deltaY);
}
void GameScreen::onMouseReleased(QMouseEvent *)
{
    m_mouseDown = false;
}

void GameScreen::onKeyPressed(QKeyEvent *e)
{
    switch (e->key())
    {
    case Qt::Key_Back:
    case Qt::Key_Backspace:
    case Qt::Key_Delete:
        m_parentApp->popScreens(1);
        break;
    default:
        m_world->onKeyPressed(e);
        break;
    }
}


// unused in game
void GameScreen::onMouseWheel(QWheelEvent *) {}
