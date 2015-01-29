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
               engine/camera engine/shapes \
               game/gamescreens game/world \
               res/images res/images/cubemap
DEPENDPATH +=  glm engine game shaders \
               engine/ui engine/screens engine/scene \
               engine/camera engine/shapes \
               game/gamescreens game/world \
               res/images res/images/cubemap
DEFINES += TIXML_USE_STL
OTHER_FILES += shaders/shader.frag shaders/shader.vert res/images/grass.png

SOURCES += engine/main.cpp \
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
    game/world/world.cpp \
    engine/screens/cubemap.cpp

HEADERS += engine/ui/mainwindow.h \
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
    game/world/world.h \
    engine/screens/cubemap.h \
    engine/printing.h
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
