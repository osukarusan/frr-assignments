TEMPLATE	= app
LANGUAGE	= C++
TARGET 		= viewer

CONFIG += debug_and_release
CONFIG += qt warn_on opengl console
CONFIG += build_all

QT += opengl

CONFIG(debug, debug|release) {
  TARGET = $$join(TARGET,,,d)
  DESTDIR = ../bin
  MOC_DIR = build/debug
  OBJECTS_DIR = build/debug
  RCC_DIR = build/debug
  UI_DIR = build/debug
  unix: LIBS += -L$$DESTDIR -lpluginglwidgetd
  win32:  LIBS += ../bin/pluginglwidgetd.lib -L../bin/ -lpluginglwidgetd
} else {
  DESTDIR = ../bin
  MOC_DIR = build/release
  OBJECTS_DIR = build/release
  RCC_DIR = build/release
  UI_DIR = build/release
  unix: LIBS += -L$$DESTDIR -lpluginglwidget
  win32:  LIBS += ../bin/pluginglwidget.lib -L../bin/ -lpluginglwidget
}

# GLEW
unix: INCLUDEPATH += ${GLEW_DIR}/include/
unix: LIBS += -L${GLEW_DIR}/lib
unix: LIBS += -lGLEW
unix: LIBS += -lcore

win32:INCLUDEPATH += ${GLEW_DIR}/include/
win32:LIBS += -L${GLEW_DIR}/lib
win32:LIBS += -lglew32
win32:LIBS += -lcore

macx:LIBS+=-lGLEW -lcore

#win32:debug {
#  LIBS += ../bin/pluginglwidgetd.lib
#  LIBS += -L../bin/ -lpluginglwidgetd
#} else {
#  LIBS += ../bin/pluginglwidget.lib
#  LIBS += -L../bin/ -lpluginglwidget
#}

#unix:debug {
#   LIBS += -L$$DESTDIR -lpluginglwidgetd
#} else {
#   LIBS += -L$$DESTDIR -lpluginglwidget
#}


INCLUDEPATH += include
INCLUDEPATH += ../glwidget/include
INCLUDEPATH += ../core/include
INCLUDEPATH += ../interfaces

HEADERS	+= include/*.h
SOURCES	+= src/*.cpp
FORMS	+= src/principal.ui

#message(Working dir: $$PWD)
#message(Target dir: $$DESTDIR)
