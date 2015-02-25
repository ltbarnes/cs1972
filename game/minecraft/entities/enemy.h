#ifndef ENEMY_H
#define ENEMY_H

#include "movableentity.h"

class Enemy : public MovableEntity
{
public:
    Enemy(glm::vec3 pos);
    virtual ~Enemy();

    void addLight(glm::vec3 pos);
    void clearLights();

    bool isStuck();

    virtual void onTick(float secs);
    virtual void handleCollision(Collision *col);

private:
    QList<glm::vec3> m_lights;

    float m_stuckTime;
    glm::vec3 m_oldPos;

};

#endif // ENEMY_H
