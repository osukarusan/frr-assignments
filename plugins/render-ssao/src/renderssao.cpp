#include "renderssao.h"
#include "glwidget.h"


RenderSSAO::RenderSSAO()
{
    normaldepthTexture = 0;
    depthRenderBuffer = 0;
    frameBuffer = 0;

    widget = new RenderSSAOWidget();
}

void RenderSSAO::paintGL()
{

    // first pass: render normals and z to the buffer
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBuffer);
    programFirst.bind();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    if (pdrawPlugin)
        pdrawPlugin->drawScene();

    programFirst.release();
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);


    // second pass: compute SSAO factor
    programSSAO.bind();

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, 1, 0, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, normaldepthTexture);
    programSSAO.setUniformValue("texture", 0);

    glBegin(GL_QUADS);
        glMultiTexCoord2f(GL_TEXTURE0, 0, 0); glVertex3f(0, 0, 0);
        glMultiTexCoord2f(GL_TEXTURE0, 1, 0); glVertex3f(1, 0, 0);
        glMultiTexCoord2f(GL_TEXTURE0, 1, 1); glVertex3f(1, 1, 0);
        glMultiTexCoord2f(GL_TEXTURE0, 0, 1); glVertex3f(0, 1, 0);
    glEnd();

    programSSAO.release();
}

void RenderSSAO::initGL()
{
    std::cerr << "DrawSSAO: initializing shaders..." << std::endl;

    programFirst.addShaderFromSourceFile(QGLShader::Vertex, "../../shaders/firstPass.vert");
    programFirst.addShaderFromSourceFile(QGLShader::Fragment, "../../shaders/firstPass.frag");
    if (!programFirst.link()) {
        std::cerr << "DrawSSAO: error initializing firstPass shader program" << endl;
        std::cerr << programFirst.log().toStdString() << std::endl;
    }

    programSSAO.addShaderFromSourceFile(QGLShader::Vertex, "../../shaders/ssao.vert");
    programSSAO.addShaderFromSourceFile(QGLShader::Fragment, "../../shaders/ssao.frag");
    if (!programFirst.link()) {
        std::cerr << "DrawSSAO: error initializing programSSAO shader program" << endl;
        std::cerr << programFirst.log().toStdString() << std::endl;
    }
}

void RenderSSAO::resizeGL(int width, int height)
{
    if (normaldepthTexture)
        glDeleteTextures(1, &normaldepthTexture);
    glGenTextures(1, &normaldepthTexture);
    glBindTexture(GL_TEXTURE_2D, normaldepthTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);

    if (depthRenderBuffer)
        glDeleteRenderbuffers(1, &depthRenderBuffer);
    glGenRenderbuffersEXT(1, &depthRenderBuffer);
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthRenderBuffer);
    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, width, height);

    if (frameBuffer)
        glDeleteFramebuffers(1, &frameBuffer);
    glGenFramebuffersEXT(1, &frameBuffer);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBuffer);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, normaldepthTexture, 0);
    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depthRenderBuffer);

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

    GLenum status;
    status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    if (status != GL_FRAMEBUFFER_COMPLETE_EXT) {
            std::cerr << "DrawSSAO: Error creating the frame buffer" << std::endl;
    }
}


Q_EXPORT_PLUGIN2(renderssao, RenderSSAO)   // plugin name, plugin class
