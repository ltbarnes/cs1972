#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <GL/glew.h>
#include <QString>
#include <QHash>
#include "camera.h"
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

    void drawQuad(glm::mat4 trans);
    void drawCube(glm::mat4 trans);

    void loadTexturesFromDirectory(const QString &dirname);
    void loadTexture(const QString &filename, const QString &key);
    static GLuint loadShaders(const char *vertex_file_path, const char *fragment_file_path);

private:
    QHash<QString, GLint> m_uniformLocs;
    QHash<QString, GLint> m_textures;

    GLuint m_defaultShader;

    Shape *m_quad;
    Shape *m_cube;

};

#endif // GRAPHICS_H
