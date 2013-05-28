TEMPLATE    = lib
TARGET      = core
DEFINES     += CORE_LIBRARY  # See Qt Ref doc, "Creating Shared Libraries"

CONFIG      += debug_and_release
CONFIG      += build_all
CONFIG      += plugin
QT          += opengl

INCLUDEPATH += include

DESTDIR     = ../bin

CONFIG(debug, debug|release) {
    TARGET  = $$join(TARGET,,,d)
    MOC_DIR = build/debug
    OBJECTS_DIR = build/debug
    RCC_DIR = build/debug
} else {
    TARGET = $$join(TARGET,,,)
    MOC_DIR = build/release
    OBJECTS_DIR = build/release
    RCC_DIR = build/release
}

# GLEW
unix:INCLUDEPATH += ${GLEW_DIR}/include
unix:LIBS += -L{GLEW_DIR}/lib -lGLEW -lGLU
win32:INCLUDEPATH += ${GLEW_DIR}/include/
win32:LIBS += -L${GLEW_DIR}/lib
win32:LIBS += -lglew32
macx:LIBS += -lGLEW -install_name $$DESTDIR/libcore.dylib

HEADERS	+= include/*.h
SOURCES	+= src/*.cpp
