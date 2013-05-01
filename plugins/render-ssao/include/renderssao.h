#ifndef _RENDERSSAO_H
#define _RENDERSSAO_H

#include "GL/glew.h"
#include "renderinterface.h"
#include "renderssaowidget.h"
#include <QGLShaderProgram>


class RenderSSAO : public QObject, public RenderInterface
{
    Q_OBJECT
    Q_INTERFACES(RenderInterface)

public:

    RenderSSAO();

    virtual void initGL();
    virtual void paintGL();
    virtual void resizeGL(int w, int h);

    virtual QWidget* getPluginWidget() { return widget; }
    virtual QString  getPluginName() { return "SSAO"; }

private:

    GLuint normaldepthTexture;
    GLuint depthRenderBuffer;
    GLuint frameBuffer;

    RenderSSAOWidget* widget;

    QGLShaderProgram programFirst;
    QGLShaderProgram programSSAO;

};
 

#endif
 
 
