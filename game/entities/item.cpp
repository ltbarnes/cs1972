#include "item.h"
#include "collisioncylinder.h"

Item::Item(glm::vec3 pos)
    : MovableEntity(pos)
{
    CollisionShape *cs;
    cs = new CollisionCylinder(glm::vec3(), glm::vec3(1.f, 1.f, 1.f));
    addCollisionShape(cs);

    RenderShape *rs;
    rs = new RenderShape();
    rs->type = CYLINDER;
    rs->color = glm::vec3(.5f, .8f, .5f);
    rs->shininess = 0.f;
    rs->trans = glm::mat4();
    rs->texture = "";
    rs->repeatU = 1.f;
    rs->repeatV = 1.f;

    addRenderShape(rs);
}

Item::~Item()
{
}


//void Item::onTick(float secs)
//{
//    MovableEntity::onTick(secs);

//    glm::vec3 pos = getPosition();

//    if (pos.y < 0.5f)
//        pos.y = 0.5f;

//    setPosition(pos);
//}
