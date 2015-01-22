#ifndef GRAPHICS_H
#define GRAPHICS_H

//#include <GL/glew.h>
#include <glu.h>
#include <QString>
#include <QHash>
#include "camera.h"

class Graphics
{
public:
    Graphics();
    ~Graphics();

    void init();
    void setUniforms(Camera *camera);

    static GLuint loadShaders(const char *vertex_file_path, const char *fragment_file_path);

private:
    QHash<QString, GLint> m_uniformLocs;

    GLuint m_default;

};

#endif // GRAPHICS_H
