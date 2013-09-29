QT += widgets opengl

HEADERS += \
	mouse.h \
    window.h \
    object.h \
    player.h \
    projectile.h \
    explosion.h
SOURCES += \
	main.cpp \
        mouse.cpp \
    window.cpp \
    object.cpp \
    player.cpp \
    projectile.cpp \
    explosion.cpp

RESOURCES += \
	mice.qrc

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/graphicsview/collidingmice
INSTALLS += target
