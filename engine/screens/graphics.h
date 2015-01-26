#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <GL/glew.h>
#include <QString>
#include <QHash>
#include "camera.h"
#include "cubemap.h"
#include "shape.h"
#include "cube.h"

class Graphics
{
public:
    Graphics();
    ~Graphics();

    void init();
    void setUniforms(Camera *camera);

    void setColor(float r, float g, float b, float a);
    void setTexture(const QString &key, float repeatU, float repeatV);

    void useCubeMap(bool use);
    bool cubeMapIsActive();
    void drawCubeMap(Camera *camera);

    void drawQuad(glm::mat4 trans);
    void drawCube(glm::mat4 trans);

    static GLuint loadShaders(const char *vertex_file_path, const char *fragment_file_path);

private:
    void loadTexturesFromDirectory();
    void loadTexture(const QString &filename, const QString &key);

    QHash<QString, GLint> m_defaultLocs;
    QHash<QString, GLint> m_cubeLocs;

    QHash<QString, GLint> m_textures;

    GLuint m_defaultShader;
    GLuint m_cubeShader;

    CubeMap *m_cubeMap;

    Shape *m_quad;
    Shape *m_cube;

    bool m_useCubeMap;

};

#endif // GRAPHICS_H
