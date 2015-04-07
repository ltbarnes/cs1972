#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include "screen.h"
#include "objecthandler.h"
#include "platformerworld.h"
#include "ellipsoid.h"
#include "navmeshhandler.h"
#include "meshbuffer.h"
#include "racer.h"
#include "course.h"

class RacerPlayer;

class GameScreen : public Screen
{
public:
    GameScreen(Application *parent, int laps);
    virtual ~GameScreen();

    // update and render
    virtual void onTick(float secs);
    virtual void onRender(Graphics *g);

    // mouse events
    virtual void onMousePressed(QMouseEvent *e);
    virtual void onMouseMoved(QMouseEvent *e, float deltaX, float deltaY);
    virtual void onMouseReleased(QMouseEvent *);

    virtual void onMouseDragged(QMouseEvent *e, float deltaX, float deltaY);
    virtual void onMouseWheel(QWheelEvent *e);

    // key events
    virtual void onKeyPressed(QKeyEvent *e);
    virtual void onKeyReleased(QKeyEvent *e);

private:
    void setWaypoints(RacerPlayer *player);
    void render2D(Graphics *g);
    glm::vec2 projectPointToScreen(glm::vec3 point, bool normalize, bool *inFrustum);

    ObjectHandler *m_oh;
    QString m_levelTexture;
    OBJ *m_level;
    NavMeshHandler *m_nmh;

    Course *m_course;

    PlatformerWorld *m_world;
    RacerPlayer *m_player;
    Racer *m_racer1;
    Racer *m_racer2;

    int m_laps[3];
    int m_maxLaps;

    bool m_drawNavMesh;

    float m_startTimer;
    int m_outcome;

    bool m_graphicsCardDestructionMode;
    MeshBuffer *m_mb;

};

#endif // GAMESCREEN_H
