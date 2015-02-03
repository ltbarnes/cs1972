#include "ground.h"
#include "collisioncylinder.h"

Ground::Ground(glm::vec3 pos)
    : StaticEntity(pos)
{
    CollisionShape *cs;
    cs = new CollisionCylinder(glm::vec3(), glm::vec3(500, 2, 500));
    addCollisionShape(cs);

    RenderShape *rs;
    rs = new RenderShape();
    rs->type = CYLINDER;
    rs->color = glm::vec3(0.75f, 0.8f, 0.9f);
    rs->shininess = 128.f;
    rs->trans = glm::scale(glm::mat4(), glm::vec3(500, 2, 500));
    rs->texture = "snow.jpg";
    rs->repeatU = 15.f;
    rs->repeatV = 15.f;
    addRenderShape(rs);
}

Ground::~Ground()
{
}
