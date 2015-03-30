#include "gamescreen.h"
#include "application.h"
#include "actioncamera.h"
#include "player.h"
#include "staticentity.h"
#include "racerplayer.h"
#include "geometriccollisionmanager.h"
#include "triangle.h"

//#include <glm/ext.hpp>

GameScreen::GameScreen(Application *parent)
    : Screen(parent)
{
    m_parentApp->setUseCubeMap(true);

    m_oh = new ObjectHandler();
    m_nmh = new NavMeshHandler();
    GLuint shader = m_parentApp->getShader(DEFAULT);

    QList<Triangle *> tris;
    QList<Triangle *> *navTris = m_nmh->getTriangleList();

    m_levelTexture = "water.jpg";
    m_level = m_oh->getObject(":/objects/level_raceway.obj", shader, &tris);
    m_nmh->setObject(m_oh->getObject(":/objects/level_raceway_navmesh.obj", shader, navTris));
    m_nmh->createVBO();

    m_world = new PlatformerWorld();
    m_world->addToMesh(tris);

    ActionCamera *cam = new ActionCamera();
    glm::vec3 playerPos = glm::vec3(0, 2, 0);

    cam->setCenter(playerPos);
    RacerPlayer *player = new RacerPlayer(cam, playerPos, glm::vec3(1, .5f, 0));

    GeometricCollisionManager *gcm = new GeometricCollisionManager();
    m_world->addManager(gcm);

    m_world->setPlayer(player);
    m_world->setGravity(glm::vec3(0, -10, 0));

    setCamera(cam);

    m_drawEllipsoid = false;
    m_drawPoint = false;
    m_mouseDown = false;
    m_drawNavMesh = false;
    m_startTimer = 3.f;

    m_ellipsoid = new Ellipsoid(glm::vec3(), glm::vec3(.25f, .5, .25f), "rayshape");

    m_racer1 = new Racer(playerPos + glm::vec3(2, 0, 2), glm::vec3(0, 1, 1));
    m_world->addMovableEntity(m_racer1);

    m_racer2 = new Racer(playerPos + glm::vec3(-2, 0,-2), glm::vec3(1, 0, 1));
    m_world->addMovableEntity(m_racer2);

    setWaypoints(player);

    m_graphicsCardDestructionMode = false;
}

GameScreen::~GameScreen()
{
    delete m_oh;
    delete m_nmh;
    delete m_world;
    delete m_ellipsoid;
}

void GameScreen::setWaypoints(RacerPlayer *player)
{
    // other racers
    QList<glm::vec3> waypoints;
    waypoints.append(glm::vec3(81, 2, -55));
    waypoints.append(glm::vec3(125, 2, 0));
    waypoints.append(glm::vec3(81, 2, 55));
    waypoints.append(glm::vec3(10, 2, -15));
    waypoints.append(glm::vec3(-81, 2, -55));
    waypoints.append(glm::vec3(-125, 2, 0));
    waypoints.append(glm::vec3(-81, 2, 55));
    waypoints.append(glm::vec3(-10, 2, -15));

    m_racer1->setWaypoints(waypoints, glm::vec3(  60, 2, -50));
    m_racer2->setWaypoints(waypoints, glm::vec3(  60, 2, -50));

    // player
    waypoints.clear();
    waypoints.append(glm::vec3(  60, 2, -50));
    waypoints.append(glm::vec3( 100, 2, -60));
    waypoints.append(glm::vec3( 130, 2, -30));
    waypoints.append(glm::vec3( 125, 2,  30));
    waypoints.append(glm::vec3( 100, 2,  58));
    waypoints.append(glm::vec3(  60, 2,  60));
    waypoints.append(glm::vec3(  30, 2,  37));
    waypoints.append(glm::vec3( -60, 2, -50));
    waypoints.append(glm::vec3(-100, 2, -60));
    waypoints.append(glm::vec3(-130, 2, -30));
    waypoints.append(glm::vec3(-125, 2,  30));
    waypoints.append(glm::vec3(-100, 2,  58));
    waypoints.append(glm::vec3( -60, 2,  60));
    waypoints.append(glm::vec3( -30, 2,  37));

    player->setWaypoints(waypoints, glm::vec3(  60, 2, -50));

}

// update and render
void GameScreen::onTick(float secs  )
{

    if (m_startTimer > 0.f)
    {
        m_startTimer -= secs;
        cout << m_startTimer << endl;
        return;
    }

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
            m_ellipsoid->setPosition(pos);
            m_nmh->setEnd(pos + glm::vec3(0, 1, 0));
        }
    }

    else if (m_drawEllipsoid)
    {
        float t2 = m_ellipsoid->intersectRayWorldSpace(p, d);
        if (t2 < t)
        {
            t = t2;
        }
    }

    if (t < INFINITY)
        m_point = p + d * t;
    else
        m_drawPoint = false;

    m_racer1->buildPath(m_nmh);
    m_racer2->buildPath(m_nmh);
}

void GameScreen::onRender(Graphics *g)
{
    if (!m_graphicsCardDestructionMode)
    {
//        m_parentApp->setUseCubeMap(true);
        g->setWorldColor(.1, .1, .1);
        g->setColor(1, 1, 1, 1, 0);


        Light light1;
        light1.type = DIRECTIONAL;
        light1.color = glm::vec3(.5f, 1, .5f);
        light1.posDir = glm::normalize(glm::vec3(-1, -1.2, .4));
        light1.id = 1;

        g->addLight(light1);



        g->setTexture("");
        g->setColor(.25f, .75f, 1, 1, 0);
        glm::mat4 trans;
        if (m_drawEllipsoid)
        {
            trans = glm::translate(glm::mat4(), m_ellipsoid->getPos()) *
                    glm::scale(glm::mat4(), m_ellipsoid->getDim() * 2.f);
            g->drawSphere(trans);
        }
        if (m_drawPoint)
        {
            trans = glm::translate(glm::mat4(), m_point) *
                    glm::scale(glm::mat4(), glm::vec3(0.1f));
            g->setColor(1, 0, 0, 1, 0);
            g->drawSphere(trans);
        }

        if (m_drawNavMesh && m_nmh->hasObject())
        {
            trans = glm::translate(glm::mat4(), glm::vec3(0, .3f, 0));

            g->setTransparentMode(true);
            trans[3][1] += 0.01f;
            m_nmh->drawPath(trans, g, glm::vec4(1, 1, 0, .3f));
            g->setColor(1, 0, 1, .3f, 0);
            trans[3][1] += 0.01f;
            m_nmh->drawEnd(trans);
            g->setColor(0, 0, 1, .3f, 0);
            m_nmh->drawStart(trans);
            g->setColor(0, 1, 1, .3f, 0);
            m_nmh->draw(trans);

            g->setColor(0, 0, 0, 1, 0);
            g->setTransparentMode(false);
            m_nmh->drawLines(trans);

            g->setTransparentMode(true);
            g->setTexture(m_levelTexture);
            g->setColor(1, 1, 1, .7f, 0);
            m_level->draw(glm::mat4());
            g->setTransparentMode(false);
        } else
        {
            g->setAllWhite(true);
            g->setTexture(m_levelTexture, 50.f, 50.f);
            m_level->draw(glm::mat4());
            g->setAllWhite(false);
        }
        m_world->onDraw(g);
    }
    else
    {
        // ray cast attempt
        g->setGraphicsMode(RAY);
        g->rayAddObjects(m_world->getObjectInfo());
        g->rayDrawQuad();
    }
}

void GameScreen::onMouseMoved(QMouseEvent *e, float deltaX, float deltaY)
{
    if (m_startTimer < 0.f)
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
    else if (e->key() == Qt::Key_G)
        m_graphicsCardDestructionMode = !m_graphicsCardDestructionMode;
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
