#include "renderdefault.h"
#include "glwidget.h"

void RenderDefault::paintGL()
{
    glClearColor(0.8f, 0.8f, 0.8f, 0.0f);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    pglwidget->drawAxes();
    if (pdrawPlugin) 
        pdrawPlugin->drawScene();

}

Q_EXPORT_PLUGIN2(renderdefault, RenderDefault)   // plugin name, plugin class
