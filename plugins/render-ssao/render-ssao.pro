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
    ../../shaders/ssaoWS.frag \
    ../../shaders/ssaoWS.vert \
    ../../shaders/ssaoSS.vert \
    ../../shaders/ssaoSS.frag \
    ../../shaders/filterGauss.frag \
    ../../shaders/filterBilateral.frag \
    ../../shaders/filterNone.frag \
    ../../shaders/filter.vert
