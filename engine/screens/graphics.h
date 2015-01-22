#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <QString>
#include <QHash>
#include <OpenGL.h>

class Graphics
{
public:
    Graphics();
    ~Graphics();

    void init();

private:
    QHash<QString, GLint> m_uniformLocs;

};

#endif // GRAPHICS_H
