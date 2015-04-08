#include "gamescreen.h"
#include "application.h"
#include "actioncamera.h"
#include "player.h"
#include "staticentity.h"
#include "racerplayer.h"
#include "collisionmanager.h"
#include "geometriccollisionmanager.h"
#include "triangle.h"

#include <glm/gtx/string_cast.hpp>

GameScreen::GameScreen(Application *parent, int laps)
    : Screen(parent)
{
    m_parentApp->setUseCubeMap(true);

    m_oh = new ObjectHandler();
    m_nmh = new NavMeshHandler();
    GLuint shader = m_parentApp->getShader(DEFAULT);

    QList<Triangle *> tris;
    QList<Triangle *> *navTris = m_nmh->getTriangleList();

    m_levelTexture = "water2.jpg";
    m_level = m_oh->getObject(":/objects/level_raceway.obj", shader, &tris);
    m_nmh->setObject(m_oh->getObject(":/objects/level_raceway_navmesh.obj", shader, navTris));
    m_nmh->createVBO();

    m_world = new PlatformerWorld();
    m_world->addToMesh(tris);

    ActionCamera *cam = new ActionCamera();
    glm::vec3 playerPos = glm::vec3(0, 1, 0);

    cam->setCenter(playerPos);
    m_player = new RacerPlayer(cam, playerPos, glm::vec3(1, .5f, 0));

    GeometricCollisionManager *gcm = new GeometricCollisionManager();
    CollisionManager *cm = new CollisionManager();
    m_world->addManager(gcm);
    m_world->addManager(cm);

    m_world->setPlayer(m_player);
    m_world->setGravity(glm::vec3(0, -10, 0));

    setCamera(cam);

    m_drawNavMesh = false;
    m_startTimer = 3.f;

    m_racer1 = new Racer(playerPos + glm::vec3(2, 0, 2), glm::vec3(0, 1, 1));
    m_world->addMovableEntity(m_racer1);

    m_racer2 = new Racer(playerPos + glm::vec3(-2, 0,-2), glm::vec3(1, 0, 1));
    m_world->addMovableEntity(m_racer2);

    setWaypoints(m_player);

    m_graphicsCardDestructionMode = false;

    m_laps[0] = 0;
    m_laps[1] = 0;
    m_laps[2] = 0;
    m_maxLaps = laps;

    m_outcome = 0;

    shader = m_parentApp->getShader(RAY);
    m_mb = new MeshBuffer();
//    m_mb->setBuffer(shader, tris);

    m_parentApp->getShader(DEFAULT);

    m_course = new Course();
}

GameScreen::~GameScreen()
{
    delete m_oh;
    delete m_nmh;
    delete m_world;
    delete m_mb;
    delete m_course;
}

void GameScreen::setWaypoints(RacerPlayer *player)
{
    // other racers
    QList<glm::vec3> waypoints;
    waypoints.append(glm::vec3(  59, 1, -60));
    waypoints.append(glm::vec3( 101, 1, -60));
    waypoints.append(glm::vec3( 130, 1, -29));
    waypoints.append(glm::vec3( 130, 1,  29));
    waypoints.append(glm::vec3( 101, 1,  60));
    waypoints.append(glm::vec3(  59, 1,  60));
    waypoints.append(glm::vec3( -59, 1, -60));
    waypoints.append(glm::vec3(-101, 1, -60));
    waypoints.append(glm::vec3(-130, 1, -29));
    waypoints.append(glm::vec3(-130, 1,  29));
    waypoints.append(glm::vec3(-101, 1,  60));
    waypoints.append(glm::vec3( -59, 1,  60));

    player->setWaypoints(waypoints, waypoints[0]);

    m_racer1->setWaypoints(waypoints, waypoints[0]);
    m_racer2->setWaypoints(waypoints, waypoints[0]);

}

// update and render
void GameScreen::onTick(float secs  )
{
    secs = glm::min(secs, .25f);

    if (m_startTimer > 0.f)
    {
        m_startTimer -= secs;
        m_player->setCameraPos();
        return;
    }

    m_world->onTick(secs);

    if (m_outcome == 0 && m_player->getPosition().y < 0.f)
    {
        m_outcome = 4;
        m_graphicsCardDestructionMode = false;
    }

    m_racer1->buildPath(m_nmh);
    m_racer2->buildPath(m_nmh);

    if (m_player->checkFinishedLap())
        m_laps[0]++;
    if (m_racer1->checkFinishedLap())
        m_laps[1]++;
    if (m_racer2->checkFinishedLap())
        m_laps[2]++;

    if (m_laps[0] == m_maxLaps)
    {
        QList<glm::vec3> waypoints;
        waypoints.append(glm::vec3(  59, 1, -60));
        m_player->setWaypoints(waypoints);
    }
    if (m_laps[1] == m_maxLaps)
    {
        QList<glm::vec3> waypoints;
        waypoints.append(glm::vec3(  59, 1, -60));
        m_racer1->setWaypoints(waypoints);
    }
    if (m_laps[2] == m_maxLaps)
    {
        QList<glm::vec3> waypoints;
        waypoints.append(glm::vec3(  59, 1, -60));
        m_racer2->setWaypoints(waypoints);
    }
    if (m_outcome == 0 && (m_laps[0] > m_maxLaps ||
                           (m_laps[2] > m_maxLaps && m_laps[1] > m_maxLaps)))
    {
        if (m_laps[1] > m_laps[0] && m_laps[2] > m_laps[0])
            m_outcome = 3;
        else if (m_laps[1] > m_laps[0] || m_laps[2] > m_laps[0])
            m_outcome = 2;
        else
            m_outcome = 1;
        m_graphicsCardDestructionMode = false;
    }
    if (m_laps[1] > m_maxLaps)
        m_racer1->setWaypoints(QList<glm::vec3>());
    if (m_laps[2] > m_maxLaps)
        m_racer2->setWaypoints(QList<glm::vec3>());
}

void GameScreen::onRender(Graphics *g)
{
    if (!m_graphicsCardDestructionMode)
    {
        g->setWorldColor(.1, .1, .1);
        g->setColor(1, 1, 1, 1, 0);

        Light light1;
        light1.type = DIRECTIONAL;
        light1.color = glm::vec3(1);
        light1.posDir = glm::normalize(glm::vec3(-0.89f, -0.41f, -0.2f));
        light1.id = 1;

        g->addLight(light1);

        g->setTexture("");
        g->setColor(.25f, .75f, 1, 1, 0);
        glm::mat4 trans;

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

//            m_course->draw(g);
        }
        m_world->onDraw(g);

        if (m_startTimer > 0.f)
        {
            glm::mat4 trans = glm::translate(glm::mat4(), glm::vec3(0,.4f,0)) *
                    glm::scale(glm::mat4(), glm::vec3(.35f));
            g->setGraphicsMode(DRAW2D);
            g->setAllWhite(true);
            if (m_startTimer > 2.f)
                g->setTexture("three.jpg");
            else if (m_startTimer > 1.f)
                g->setTexture("two.jpg");
            else
                g->setTexture("one.jpg");
            g->drawQuad(trans);
            g->setAllWhite(false);
        }
        if (m_outcome > 0)
        {
            glm::mat4 trans = glm::scale(glm::mat4(), glm::vec3(.7f, .75f, .1f));
            g->setGraphicsMode(DRAW2D);
            g->setAllWhite(true);
            if (m_outcome == 4)
                g->setTexture("death.png");
            else if (m_outcome == 3)
                g->setTexture("third.png");
            else if (m_outcome == 2)
                g->setTexture("second.png");
            else
                g->setTexture("first.png");
            g->drawQuad(trans);
            g->setAllWhite(false);
        }
        else
            this->render2D(g);
    }
    else
    {
        // ray cast attempt
        GLuint shader = g->setGraphicsMode(RAY);

        ObjectsInfo* oi = m_world->getObjectInfo();
        int objSize = oi->invs.size();
        g->rayAddObjects(oi);

        oi = m_course->getObjectInfo();
        int cylSize = oi->invs.size();
        g->rayAddObjects(oi, objSize);

        glUniform1i(glGetUniformLocation(shader, "NUM_OBJECTS"), objSize);
        glUniform1i(glGetUniformLocation(shader, "NUM_CYLS"), cylSize);

        g->rayAddTransparents(m_player->getWaypointInfo());
        g->rayDrawQuad();
    }
}

void GameScreen::render2D(Graphics *g)
{
    glm::mat4 trans;
    glm::vec3 wp = m_player->getCurrentWaypoint();

    g->setGraphicsMode(DRAW2D);
    g->setTexture("");

    bool inFrustum;
    glm::vec2 screenP;

    // waypoints set
    if (glm::dot(wp, wp) >= 0.0001f)
    {
        screenP = projectPointToScreen(wp, true, &inFrustum);

        if (!inFrustum)
        {
            float dot = glm::dot(glm::vec2(0, 1), screenP);
            float angle = std::atan2(-screenP.x, dot);
            trans = glm::rotate(glm::mat4(), angle, glm::vec3(0, 0, 1));
            trans *= glm::translate(glm::mat4(), glm::vec3(0, .95f, 0));
            trans *= glm::scale(glm::mat4(), glm::vec3(.05f));

            g->drawCone(trans);
        }
        else
        {
            wp.y += m_player->getWaypointRadius() + 1.5f + std::sin(g->getElapsedTime() * .01);
            screenP = projectPointToScreen(wp, false, &inFrustum);

            trans = glm::translate(glm::mat4(), glm::vec3(screenP, 0));
            trans *= glm::rotate(glm::mat4(), glm::pi<float>(), glm::vec3(0, 0, 1));
            trans *= glm::scale(glm::mat4(), glm::vec3(.05f));

            g->drawCone(trans);
        }
    }
}


glm::vec2 GameScreen::projectPointToScreen(glm::vec3 point, bool normalize, bool *inFrustum)
{
    // normalized screen space
    glm::vec4 sp = m_camera->getProjectionMatrix() *
                   m_camera->getViewMatrix() * glm::vec4(point, 1);
    sp *= 1.f / glm::abs(sp.w);

    // check if waypoint is within frustum
    *inFrustum = sp.z > 0 && sp.z < 1 && sp.x >= -1 && sp.x <= 1 && sp.y >= -1 && sp.y <= 1;

    if (normalize)
        return glm::normalize(glm::vec2(sp));
    return glm::vec2(sp);
}


void GameScreen::onMouseMoved(QMouseEvent *e, float deltaX, float deltaY)
{
    if (m_startTimer < 0.f && m_outcome == 0)
        m_world->onMouseMoved(e, deltaX, deltaY);
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
    if (m_startTimer < 0.f && m_outcome == 0)
        m_world->onMouseMoved(e, deltaX, deltaY);
}

void GameScreen::onKeyPressed(QKeyEvent *e)
{
    if (m_outcome > 0)
    {
        switch (e->key())
        {
        case Qt::Key_W:
        case Qt::Key_A:
        case Qt::Key_S:
        case Qt::Key_D:
        case Qt::Key_G:
        case Qt::Key_N:
            break;
        default:
            m_parentApp->popScreens(1);
            break;
        }
        return;
    }

    switch (e->key())
    {
    case Qt::Key_Back:
    case Qt::Key_Backspace:
    case Qt::Key_Delete:
        m_parentApp->popScreens(1);
        break;
    default:
        if (m_startTimer < 0.f && m_outcome == 0)
            m_world->onKeyPressed(e);
        break;
    }
}


// unused in game
void GameScreen::onMousePressed(QMouseEvent *) {}
void GameScreen::onMouseReleased(QMouseEvent *) {}
void GameScreen::onMouseWheel(QWheelEvent *) {}
