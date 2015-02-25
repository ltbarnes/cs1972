#include "application.h"

Application::Application()
{
    m_currentScreen = NULL;

    // create graphics object
    m_g = new Graphics();
}

Application::~Application()
{
    if (m_currentScreen)
        delete m_currentScreen;

    foreach(Screen *s, m_screens)
        delete s;

    delete m_g;
}

void Application::init(Screen *initScreen)
{
    m_currentScreen = initScreen;
    m_g->init();
}

void Application::addScreen(Screen *s)
{
    if (m_currentScreen)
        m_screens.append(m_currentScreen);

    m_currentScreen = s;
    m_currentScreen->onResize(m_width, m_height);
}

void Application::popScreens(int num)
{
    while (num-- > 0)
    {
        if (m_currentScreen)
            delete m_currentScreen;

        if (!m_screens.isEmpty())
            m_currentScreen = m_screens.takeLast();
        else
        {
            m_currentScreen = NULL;
            break;
        }
    }
}

void Application::onTick(float secs)
{
    m_g->update();

    if (m_currentScreen)
        m_currentScreen->onTick(secs);
}

void Application::onRender()
{
    if (m_currentScreen)
    {
        if (m_g->cubeMapIsActive())
            m_g->drawCubeMap(m_currentScreen->getCamera());

        m_g->setGraphicsMode(DEFAULT);
        m_g->setCamera(m_currentScreen->getCamera());
        m_g->setColor(0.f, 0.f, 0.f, 1.f, 1.f);
        m_currentScreen->onRender(m_g);

    }
}

void Application::setUseCubeMap(bool use)
{
    m_g->useCubeMap(use);
}

void Application::onMousePressed(QMouseEvent *e)
{
    if (m_currentScreen)
        m_currentScreen->onMousePressed(e);
}

void Application::onMouseMoved(QMouseEvent *e, float deltaX, float deltaY)
{
    if (m_currentScreen)
        m_currentScreen->onMouseMoved(e, deltaX, deltaY);
}

void Application::onMouseReleased(QMouseEvent *e)
{
    if (m_currentScreen)
        m_currentScreen->onMouseReleased(e);
}

void Application::onMouseDragged(QMouseEvent *e, float deltaX, float deltaY)
{
    if (m_currentScreen)
        m_currentScreen->onMouseDragged(e, deltaX, deltaY);
}

void Application::onMouseWheel(QWheelEvent *e)
{
    if (m_currentScreen)
        m_currentScreen->onMouseWheel(e);
}

void Application::onKeyPressed(QKeyEvent *e)
{
    if (m_currentScreen)
        m_currentScreen->onKeyPressed(e);
}

void Application::onKeyReleased(QKeyEvent *e)
{
    if (m_currentScreen)
        m_currentScreen->onKeyReleased(e);
}


void Application::onResize(int w, int h)
{
    if (m_currentScreen)
        m_currentScreen->onResize(w, h);

    m_width = w;
    m_height = h;
}

GLuint Application::getShader(GraphicsMode gm)
{
    return m_g->setGraphicsMode(gm);
}


