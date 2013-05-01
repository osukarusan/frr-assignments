TARGET     = $$qtLibraryTarget(renderssao)

include(../common.pro)

HEADERS += \
    renderssaowidget.h

SOURCES += \
    renderssaowidget.cpp

FORMS += \
    renderssaowidget.ui

OTHER_FILES += \
    ../../shaders/firstPass.vert \
    ../../shaders/firstPass.frag \
    ../../shaders/ssao.vert \
    ../../shaders/ssao.frag
