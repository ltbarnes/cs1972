#ifndef VIEW_H
#define VIEW_H

#include <glu.h>
#include <qgl.h>
#include <QTime>
#include <QTimer>
#include <glm.h>
#include "application.h"

class View : public QGLWidget
{
    Q_OBJECT

public:
    View(QGLFormat format, QWidget *parent);
    ~View();

private:
    QTime time;
    QTimer timer;
    float fps;

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void wheelEvent(QWheelEvent *event);

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private slots:
    void tick();

private:
    Application *m_app;
    bool m_mouseDown;

    Graphics *m_g;
};

#endif // VIEW_H

