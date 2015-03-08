QT += core gui opengl

TARGET = cs1972
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
               engine/common engine/ui engine/graphics \
               engine/shapes engine/world engine/voxel \
               engine/collisions \
               # game/warmup1/gamescreens game/warmup1/world game/warmup1/entities \ # warmup
               # game/minecraft/gamescreens game/minecraft/world game/minecraft/entities \ # minecraft
               game/platformer/gamescreens game/platformer/world game/platformer/entities \ # platformer
               res/images res/images/cubemap res/levels
DEPENDPATH +=  glm engine game shaders \
               engine/common engine/ui engine/graphics \
               engine/shapes engine/world engine/voxel \
               engine/collisions \
               # game/warmup1/gamescreens game/warmup1/world game/warmup1/entities \ # warmup
               # game/minecraft/gamescreens game/minecraft/world game/minecraft/entities \ # minecraft
               game/platformer/gamescreens game/platformer/world game/platformer/entities \ # platformer
               res/images res/images/cubemap res/levels
DEFINES += TIXML_USE_STL
OTHER_FILES += shaders/shader.frag shaders/shader.vert

SOURCES += \
    engine/main.cpp \
    engine/ui/mainwindow.cpp \
    engine/ui/view.cpp \
    engine/ui/application.cpp \
    engine/ui/button.cpp \
    engine/common/point3d.cpp \
    engine/common/obj.cpp \
    engine/graphics/graphics.cpp \
    engine/graphics/cubemap.cpp \
    engine/graphics/camera.cpp \
    engine/graphics/actioncamera.cpp \
    engine/world/world.cpp \
    engine/world/entity.cpp \
    engine/world/movableentity.cpp \
    engine/world/staticentity.cpp \
    engine/shapes/shape.cpp \
    engine/shapes/cube.cpp \
    engine/shapes/cylinder.cpp \
    engine/shapes/sphere.cpp \
    engine/shapes/cone.cpp \
    engine/shapes/facecube.cpp \
    engine/collisions/collisioncylinder.cpp \
    engine/voxel/chunk.cpp \
    engine/voxel/voxelmanager.cpp \
    engine/common/perlinnoise.cpp \
    engine/graphics/particleemitter.cpp \
    engine/collisions/collisionmanager.cpp \
### platformer
    game/platformer/gamescreens/platformermenu.cpp \
    game/platformer/gamescreens/gamescreen.cpp
### minecraft
#    game/minecraft/gamescreens/minecraftmenu.cpp \
#    game/minecraft/gamescreens/gamescreen.cpp \
#    game/minecraft/gamescreens/createscreen.cpp \
#    game/minecraft/gamescreens/gameoverscreen.cpp \
#    game/minecraft/world/minecraftworld.cpp \
#    game/minecraft/world/mcchunkbuilder.cpp \
#    game/minecraft/entities/player.cpp \
#    game/minecraft/entities/ally.cpp \
#    game/minecraft/entities/enemy.cpp \
#    game/minecraft/world/mccollisionmanager.cpp
### warmup
#    game/warmup/gamescreens/menuscreen.cpp \
#    game/warmup/gamescreens/gamescreen.cpp \
#    game/warmup/gamescreens/endgamescreen.cpp \
#    game/warmup/world/gameworld.cpp \
#    game/warmup/entities/player.cpp \
#    game/warmup/entities/item.cpp \
#    game/warmup/entities/ufo.cpp \
#    game/warmup/entities/ground.cpp

HEADERS += \
    engine/ui/mainwindow.h \
    engine/ui/view.h \
    engine/ui/application.h \
    engine/ui/screen.h \
    engine/ui/button.h \
    engine/common/point3d.h \
    engine/common/obj.h \
    engine/graphics/graphics.h \
    engine/graphics/cubemap.h \
    engine/graphics/camera.h \
    engine/graphics/actioncamera.h \
    engine/world/world.h \
    engine/world/entity.h \
    engine/world/movableentity.h \
    engine/world/staticentity.h \
    engine/shapes/shape.h \
    engine/shapes/cube.h \
    engine/shapes/cylinder.h \
    engine/shapes/sphere.h \
    engine/shapes/cone.h \
    engine/shapes/facecube.h \
    engine/collisions/collisionshape.h \
    engine/collisions/collisioncylinder.h \
    engine/voxel/chunk.h \
    engine/voxel/voxelmanager.h \
    engine/voxel/chunkbuilder.h \
    engine/world/manager.h \
    engine/common/perlinnoise.h \
    engine/graphics/particleemitter.h \
    engine/collisions/collisionmanager.h \
### platformer
    game/platformer/gamescreens/platformermenu.h \
    game/platformer/gamescreens/gamescreen.h
### minecraft
#    game/minecraft/gamescreens/minecraftmenu.h \
#    game/minecraft/gamescreens/gamescreen.h \
#    game/minecraft/gamescreens/createscreen.h \
#    game/minecraft/gamescreens/gameoverscreen.h \
#    game/minecraft/world/minecraftworld.h \
#    game/minecraft/world/mcchunkbuilder.h \
#    game/minecraft/entities/player.h \
#    game/minecraft/entities/ally.h \
#    game/minecraft/entities/enemy.h \
#    game/minecraft/world/mccollisionmanager.h
### warmup
#    game/warmup/gamescreens/menuscreen.h \
#    game/warmup/gamescreens/gamescreen.h \
#    game/warmup/gamescreens/endgamescreen.h \
#    game/warmup/world/gameworld.h \
#    game/warmup/entities/player.h \
#    game/warmup/entities/item.h \
#    game/warmup/entities/ufo.h \
#    game/warmup/entities/ground.h

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
