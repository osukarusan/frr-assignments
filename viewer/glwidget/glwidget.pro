TEMPLATE   = lib

TARGET     = pluginglwidget

CONFIG     += designer plugin 
CONFIG     += debug_and_release
CONFIG     += build_all

QT         += opengl

INCLUDEPATH += include
INCLUDEPATH += ../core/include
INCLUDEPATH += ../interfaces

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

unix:target.path = ~/.designer/plugins/designer
win32:target.path = $$[QT_INSTALL_PLUGINS]/designer

# GLEW
unix:INCLUDEPATH += ${GLEW_DIR}/include
unix:LIBS += -L../bin -lcore

win32:INCLUDEPATH += ${GLEW_DIR}/include/
win32:LIBS += -L${GLEW_DIR}/lib
win32:LIBS += -lglew32
win32:LIBS += -L../bin/ -lcore

macx:LIBS +=  -L../bin/ -lGLEW -lcore -install_name $$DESTDIR/libpluginglwidget.dylib
INSTALLS   += target

DEFINES += PLUGINGLWIDGET_LIBRARY   # see Qt docs, "Creating shared libraries"

HEADERS += include/*.h 
SOURCES	+= src/*.cpp

