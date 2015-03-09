#include "gamescreen.h"
#include "application.h"
#include "actioncamera.h"
#include "player.h"

GameScreen::GameScreen(Application *parent, int level)
    : Screen(parent)
{
    m_parentApp->setUseCubeMap(true);
    m_level = new OBJ();

    switch (level)
    {
    case 2:
        m_level->read(":/levels/level_hard.obj");
        break;
    case 3:
        m_level->read(":/levels/level_island.obj");
        break;
    default: // 1
        m_level->read(":/levels/level_easy.obj");
        break;
    }
    m_level->createVBO(m_parentApp->getShader(DEFAULT));


    ActionCamera *cam = new ActionCamera();
    cam->setCenter(glm::vec3(0, 2, 0));

    player = new Player(cam, glm::vec3());

    setCamera(cam);
}

GameScreen::~GameScreen()
{
    delete m_level;
    delete player;
}

// update and render
void GameScreen::onTick(float secs  )
{
    player->onTick(secs);

    player->setCameraPos();
}

void GameScreen::onRender(Graphics *g)
{
    g->setWorldColor(.1, .1, .1);
    g->setColor(1, 1, 1, 1, 64);

    Light light1;
    light1.type = DIRECTIONAL;
    light1.color = glm::vec3(.5f, 1, .5f);
    light1.posDir = glm::vec3(1, -1, 1);
    light1.id = 1;

    Light light2;
    light2.type = DIRECTIONAL;
    light2.color = glm::vec3(.5f, .5f, 1);
    light2.posDir = glm::vec3(-1, -1, -1);
    light2.id = 2;

    g->addLight(light1);
//    g->addLight(light2);

    m_level->draw(glm::mat4());
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
