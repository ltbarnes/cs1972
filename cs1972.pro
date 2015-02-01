QT += core gui opengl

TARGET = warmup
TEMPLATE = app

unix:!macx {
    LIBS += -lGLU
    QMAKE_CXXFLAGS += -std=c++11
}
macx {
    QMAKE_CFLAGS_X86_64 += -mmacosx-version-min=10.7
    QMAKE_CXXFLAGS_X86_64 = $$QMAKE_CFLAGS_X86_64
    QMAKE_MAC_SDK = macosx10.9
}

# If you add your own folders, add them to INCLUDEPATH and DEPENDPATH, e.g.
INCLUDEPATH += glm engine game shaders \
               engine/ui engine/screens engine/scene \
               engine/camera engine/shapes engine/world \
               game/gamescreens game/world game/entities \
               res/images res/images/cubemap
DEPENDPATH +=  glm engine game shaders \
               engine/ui engine/screens engine/scene \
               engine/camera engine/shapes engine/world \
               game/gamescreens game/world game/entities \
               res/images res/images/cubemap
DEFINES += TIXML_USE_STL
OTHER_FILES += shaders/shader.frag shaders/shader.vert res/images/grass.png

SOURCES += \
    engine/main.cpp \
    engine/ui/mainwindow.cpp \
    engine/ui/view.cpp \
    engine/screens/application.cpp \
    engine/screens/graphics.cpp \
    engine/camera/camera.cpp \
    engine/screens/openglscreen.cpp \
    engine/shapes/shape.cpp \
    game/gamescreens/menuscreen.cpp \
    game/gamescreens/playerscreen.cpp \
    engine/shapes/cube.cpp \
    engine/screens/cubemap.cpp \
    game/entities/player.cpp \
    engine/camera/actioncamera.cpp \
    engine/world/world.cpp \
    engine/world/entity.cpp \
    engine/world/movableentity.cpp \
    game/world/gameworld.cpp \
    engine/shapes/cylinder.cpp \
    engine/shapes/sphere.cpp \
    engine/shapes/collisioncylinder.cpp \
    game/entities/item.cpp \
    engine/world/staticentity.cpp \
    game/entities/ground.cpp \
    game/entities/ufo.cpp

HEADERS += \
    engine/ui/mainwindow.h \
    engine/ui/view.h \
    engine/screens/application.h \
    engine/screens/graphics.h \
    engine/screens/screen.h \
    engine/camera/camera.h \
    engine/screens/openglscreen.h \
    engine/shapes/shape.h \
    game/gamescreens/menuscreen.h \
    game/gamescreens/playerscreen.h \
    engine/shapes/cube.h \
    engine/screens/cubemap.h \
    engine/printing.h \
    game/entities/player.h \
    engine/camera/actioncamera.h \
    engine/world/world.h \
    engine/world/entity.h \
    engine/world/movableentity.h \
    game/world/gameworld.h \
    engine/shapes/cylinder.h \
    engine/shapes/sphere.h \
    engine/shapes/collisionshape.h \
    engine/shapes/collisioncylinder.h \
    game/entities/item.h \
    engine/world/staticentity.h \
    game/entities/ground.h \
    game/entities/ufo.h
#    engine/vector/vector.h

FORMS += engine/ui/mainwindow.ui

#TODO (Windows): If you are setting up local development on Windows (NOT Mac), comment out the following lines
win32:CONFIG(release, debug|release): LIBS += -L/course/cs123/lib/glew/glew-1.10.0/lib/release/ -lGLEW
else:win32:CONFIG(debug, debug|release): LIBS += -L/course/cs123/lib/glew/glew-1.10.0/lib/debug/ -lGLEW
else:unix: LIBS += -L/usr/local/Cellar/glew/1.11.0/lib/ -lGLEW

#TODO (Windows or Mac): If you are setting up local development on Windows OR Mac, fill in the correct path to your glew and uncomment the following lines:
INCLUDEPATH+=/usr/local/Cellar/glew/1.11.0/include
DEPENDPATH+=/usr/local/Cellar/glew/1.11.0/include

RESOURCES += \
    resources.qrc
