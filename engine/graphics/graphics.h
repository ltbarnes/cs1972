#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <GL/glew.h>
#include <QString>
#include <QHash>
#include "camera.h"
#include "cubemap.h"
#include "shape.h"
#include "cone.h"
#include "cube.h"
#include "cylinder.h"
#include "sphere.h"
#include "facecube.h"

enum GraphicsMode
{
    DEFAULT, SPARSE, CUBEMAP
};

enum ShapeType
{
    QUAD, CUBE, CYLINDER, SPHERE, CONE
};

struct RenderShape
{
    ShapeType type;
    glm::vec3 color;
    float shininess;
    float transparency;
    glm::mat4 trans;
    QString texture;
    float repeatU, repeatV;
};

// Data for a single light
struct Light
{
   int id;

   glm::vec3 color;
   glm::vec3 pos;       // Not applicable to directional lights

   glm::vec3 function;  // Attenuation function
};

class Graphics
{
public:
    Graphics();
    ~Graphics();

    void init();
    void setCamera(Camera *camera);
    void setGraphicsMode(GraphicsMode gm);

    void setWorldColor(float r, float g, float b);
    void setColor(float r, float g, float b, float transparency, float shininess);
    void setTexture(const QString &key, float repeatU = 1.f, float repeatV = 1.f);
    void setAtlas(const QString &key, glm::vec2 numSubImages);
    void setAtlasPosition(float x, float y);
    void setTransparentMode(bool on);

    void useCubeMap(bool use);
    bool cubeMapIsActive();
    void drawCubeMap(Camera *camera);

    void addLight(const Light &light);

    void drawQuad(glm::mat4 trans);
    void drawCone(glm::mat4 trans);
    void drawCube(glm::mat4 trans);
    void drawCyl(glm::mat4 trans);
    void drawSphere(glm::mat4 trans);
    void drawFaceCube(glm::mat4 trans, int info);

    static GLuint loadShaders(const char *vertex_file_path, const char *fragment_file_path);

private:
    void clearLights();

    void loadTexturesFromDirectory();
    void loadTexture(const QString &filename, const QString &key);

    glm::vec3 m_lightPos;

    QHash<QString, GLint> m_defaultLocs;
    QHash<QString, GLint> m_sparseLocs;
    QHash<QString, GLint> m_cubeLocs;

    QHash<QString, GLint> m_textures;

    GLuint m_currentShader;
    GLuint m_defaultShader;
    GLuint m_sparseShader;
    GLuint m_cubeShader;

    CubeMap *m_cubeMap;

    glm::mat4 m_currProj;
    glm::mat4 m_currView;

    Shape *m_quad;
    Shape *m_cone;
    Shape *m_cube;
    Shape *m_cyl;
    Shape *m_sphere;
    Shape *m_faceCube;

    bool m_useCubeMap;
    bool m_usingAtlas;

    glm::vec2 m_subImages;

//    glm::mat4 m_p, m_v;

};

#endif // GRAPHICS_H
