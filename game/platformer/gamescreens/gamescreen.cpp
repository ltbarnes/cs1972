#include "gamescreen.h"
#include "application.h"
#include "actioncamera.h"
#include "player.h"
#include "staticentity.h"
#include "platformerplayer.h"
#include "geometriccollisionmanager.h"
#include "ellipsoid.h"

GameScreen::GameScreen(Application *parent, int level)
    : Screen(parent)
{
    m_oh = new ObjectHandler();
    GLuint shader = m_parentApp->getShader(DEFAULT);

    QList<Triangle *> tris;

    switch (level)
    {
    case 2:
        m_levelTexture = "level_hard.png";
        m_level = m_oh->getObject(":/objects/level_hard.obj", shader, &tris);
        break;
    case 3:
        m_levelTexture = "level_island.png";
        m_level = m_oh->getObject(":/objects/level_island.obj", shader, &tris);
        break;
    default: // 1
        m_levelTexture = "level_easy.png";
        m_level = m_oh->getObject(":/objects/level_easy.obj", shader, &tris);
        break;
    }

    m_world = new PlatformerWorld();
    m_world->addToMesh(tris);

    ActionCamera *cam = new ActionCamera();
    cam->setCenter(glm::vec3(0, 2, 0));

    glm::vec3 playerPos = glm::vec3(0, 2, 0);
    PlatformerPlayer *player = new PlatformerPlayer(cam, playerPos);

    GeometricCollisionManager *gcm = new GeometricCollisionManager();
    m_world->addManager(gcm);

    Ellipsoid *e = new Ellipsoid(glm::vec3(0, 0, 0), glm::vec3(.49f, .98f, .49f), "player");
    e->updatePos(playerPos);
    player->addCollisionShape(e);

    RenderShape *rs = new RenderShape();
    rs->type = SPHERE;
    rs->color = glm::vec3(0, 1.f, 0);
    rs->shininess = 32.f;
    rs->transparency = 1.f;
    rs->trans = glm::scale(glm::mat4(), glm::vec3(.99f, 1.98f, .99f));
    rs->texture = "";
    rs->repeatU = 1.f;
    rs->repeatV = 1.f;
    player->addRenderShape(rs);

    m_world->setPlayer(player);
    m_world->setGravity(glm::vec3(0, -10, 0));

    setCamera(cam);
}

GameScreen::~GameScreen()
{
    delete m_oh;
    delete m_world;
}

// update and render
void GameScreen::onTick(float secs  )
{
    m_world->onTick(secs);
}

void GameScreen::onRender(Graphics *g)
{
    g->setWorldColor(.1, .1, .1);
    g->setColor(1, 1, 1, 1, 0);

    Light light1;
    light1.type = DIRECTIONAL;
    light1.color = glm::vec3(.5f, 1, .5f);
    light1.posDir = glm::vec3(1, 1, 1);
    light1.id = 1;

//    g->addLight(light1);

    g->setAllWhite(true);
    g->setTexture(m_levelTexture);
    m_level->draw(glm::mat4());
    g->setAllWhite(false);

     m_world->onDraw(g);
}

void GameScreen::onMouseMoved(QMouseEvent *e, float deltaX, float deltaY)
{
    m_world->onMouseMoved(e, deltaX, deltaY);
}

void GameScreen::onMousePressed(QMouseEvent *)
{
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

void GameScreen::onKeyReleased(QKeyEvent *e )
{
    m_world->onKeyReleased(e);
}

// unused in game
void GameScreen::onMouseReleased(QMouseEvent *) {}
void GameScreen::onMouseDragged(QMouseEvent *, float, float) {}
void GameScreen::onMouseWheel(QWheelEvent *) {}
