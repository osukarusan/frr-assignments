
HEADERS	+= include/*.h
SOURCES	+= src/*.cpp

TEMPLATE   = lib
CONFIG     += plugin 
CONFIG     += debug_and_release
CONFIG     += build_all

QT         += opengl

INCLUDEPATH += include
INCLUDEPATH += ../../viewer/glwidget/include
INCLUDEPATH += ../../viewer/interfaces
INCLUDEPATH += ../../viewer/core/include

DESTDIR = ../bin

CONFIG(debug, debug|release) {
  TARGET = $$join(TARGET,,,d)
  MOC_DIR = build/debug
  OBJECTS_DIR = build/debug
  RCC_DIR = build/debug
} else {
  TARGET = $$join(TARGET,,,)
  MOC_DIR = build/release
  OBJECTS_DIR = build/release
  RCC_DIR = build/release
}

unix:target.path = ../bin/plugins
win32:target.path = ../bin/plugins

# GLEW
unix:INCLUDEPATH += ${GLEW_DIR}/include
unix:LIBS += -L${GLEW_DIR}/lib -lGLEW
win32:INCLUDEPATH += ${GLEW_DIR}/include/
win32:LIBS += -L${GLEW_DIR}/lib
win32:LIBS += -lglew32
INSTALLS   += target

win32:LIBS += -L../../viewer/bin/ -lcore
win32:LIBS += ../../viewer/bin/pluginglwidget.lib -L../bin/ -lpluginglwidget

macx: LIBS += -L../../viewer/bin/ -lcore -lGLEW
macx: LIBS += -L../bin/ -lpluginglwidget






