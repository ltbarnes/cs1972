#include "gamescreen.h"
#include "application.h"
#include "actioncamera.h"
#include "player.h"
#include "staticentity.h"
#include "platformerplayer.h"

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
        m_level = m_oh->getObject(":/objects/level_hard.obj", shader, tris);
        break;
    case 3:
        m_levelTexture = "level_island.png";
        m_level = m_oh->getObject(":/objects/level_island.obj", shader, tris);
        break;
    default: // 1
        m_levelTexture = "level_easy.png";
        m_level = m_oh->getObject(":/objects/level_easy.obj", shader, tris);
        break;
    }

    m_world = new PlatformerWorld();

    ActionCamera *cam = new ActionCamera();
    cam->setCenter(glm::vec3(0, 2, 0));

    player = new PlatformerPlayer(cam, glm::vec3());

    setCamera(cam);
}

GameScreen::~GameScreen()
{
    delete m_oh;
    delete player;
    delete m_world;
}

// update and render
void GameScreen::onTick(float secs  )
{
    // should be world->onTick(secs)
    player->onTick(secs);

    player->setCameraPos();
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

    // world.onDraw(g);
}

void GameScreen::onMouseMoved(QMouseEvent *e, float deltaX, float deltaY)
{
    player->onMouseMoved(e, deltaX, deltaY);
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
        // should be world->onKP(e);
        player->onKeyPressed(e);
        break;
    }
}

void GameScreen::onKeyReleased(QKeyEvent *e )
{
    player->onKeyReleased(e);
}

// unused in game
void GameScreen::onMouseReleased(QMouseEvent *) {}
void GameScreen::onMouseDragged(QMouseEvent *, float, float) {}
void GameScreen::onMouseWheel(QWheelEvent *) {}
