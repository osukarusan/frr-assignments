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

    enum SampleMode { WORLD_SPACE, SCREEN_SPACE };
    enum FilterMode { NO_FILTER, GAUSSIAN_BLUR, BILATERAL_BLUR };

    RenderSSAO();
    ~ RenderSSAO();

    virtual void onPluginLoad();

    virtual void initGL();
    virtual void paintGL();
    virtual void resizeGL(int w, int h);

    virtual QWidget* getPluginWidget() { return widget; }
    virtual QString  getPluginName() { return "SSAO"; }

public slots:

    void setSampleMode(SampleMode sm);
    void setSampleRadiusSize(float r);
    void setFilterMode(FilterMode fm);
    void setFilterRadiusSize(float r);
    void reloadShaders();

private:

    void generateSamplingPattern();
    void generateRotationPattern();

    GLuint normaldepthTexture, aoTexture;
    GLuint depthRenderBuffer;
    GLuint frameBuffer1, frameBuffer2;

    QVector3D samplingPattern[16];
    GLuint samplingPatternTexture;
    GLuint rotationPatternTexture;

    QGLShaderProgram programFirst;
    QGLShaderProgram programSSAO;
    QGLShaderProgram programFilter;

    SampleMode sampleMode;
    float      sampleRadiusSize;
    FilterMode filterMode;
    float      filterRadiusSize;

    RenderSSAOWidget* widget;

};
 

#endif
 
 
