#ifndef APPLICATION_H
#define APPLICATION_H

#include "screen.h"

class Application
{
public:
    Application(Screen *initScreen);
    ~Application();

    void init();

    // set the current screen
    void addScreen(Screen *s);
    void popScreen();

    // update and render
    void onTick(float secs);
    void onRender();

    // mouse events
    void onMousePressed(QMouseEvent *e);
    void onMouseMoved(QMouseEvent *e);
    void onMouseReleased(QMouseEvent *e);

    void onMouseDragged(QMouseEvent *e);
    void onMouseWheel(QWheelEvent *e);

    // key events
    void onKeyPressed(QKeyEvent *e);
    void onKeyReleased(QKeyEvent *e);

    // resize
    void onResize(int w, int h);

private:
    QList<Screen *> m_screens;
    Screen *m_currentScreen;

    Graphics *m_g;

};

#endif // APPLICATION_H
