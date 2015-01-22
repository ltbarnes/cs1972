#include "application.h"

Application::Application(Screen *initScreen)
{
    m_currentScreen = initScreen;
}

Application::~Application()
{
    if (m_currentScreen)
        delete m_currentScreen;

    foreach(Screen *s, m_screens)
        delete s;
}

void Application::addScreen(Screen *s)
{
    if (m_currentScreen)
        m_screens.append(s);

    m_currentScreen = s;
}

void Application::popScreen()
{
    delete m_currentScreen;
    if (!m_screens.isEmpty())
        m_currentScreen = m_screens.takeLast();
    else
        m_currentScreen = NULL;
}

void Application::onTick(float secs)
{
    if (m_currentScreen)
        m_currentScreen->onTick(secs);
}

void Application::onRender(Graphics *g)
{
    if (m_currentScreen)
        m_currentScreen->onRender(g);
}

void Application::onMousePressed(QMouseEvent *e)
{
    if (m_currentScreen)
        m_currentScreen->onMousePressed(e);
}

void Application::onMouseMoved(QMouseEvent *e)
{
    if (m_currentScreen)
        m_currentScreen->onMouseMoved(e);
}

void Application::onMouseReleased(QMouseEvent *e)
{
    if (m_currentScreen)
        m_currentScreen->onMouseReleased(e);
}

void Application::onMouseDragged(QMouseEvent *e)
{
    if (m_currentScreen)
        m_currentScreen->onMouseDragged(e);
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
}


