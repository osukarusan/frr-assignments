#include "renderssao.h"
#include "glwidget.h"
#include <cmath>

RenderSSAO::RenderSSAO()
{
    normaldepthTexture = 0;
    aoTexture = 0;
    samplingPatternTexture = 0;
    rotationPatternTexture = 0;
    depthRenderBuffer = 0;
    frameBuffer1 = 0;
    frameBuffer2 = 0;

    sampleMode = SCREEN_SPACE;
    filterMode = BILATERAL_BLUR;
    sampleRadiusSize = 0.5f;
    filterRadiusSize = 2.5f;
}

RenderSSAO::~RenderSSAO() {
    if (normaldepthTexture)
        glDeleteTextures(1, &normaldepthTexture);
    if (aoTexture)
        glDeleteTextures(1, &aoTexture);
    if (samplingPatternTexture)
        glDeleteTextures(1, &samplingPatternTexture);
    if (rotationPatternTexture)
        glDeleteTextures(1, &rotationPatternTexture);
    if (depthRenderBuffer)
        glDeleteRenderbuffers(1, &depthRenderBuffer);
    if (frameBuffer1)
        glDeleteFramebuffers(1, &frameBuffer1);
    if (frameBuffer2)
        glDeleteFramebuffers(1, &frameBuffer2);
}

void RenderSSAO::onPluginLoad()
{
    RenderSSAOWidget* aux = new RenderSSAOWidget();
    aux->setRendererPointer(this);
    widget = aux;
}

void RenderSSAO::paintGL()
{
    // first pass: render normals and z to the buffer
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBuffer1);
    programFirst.bind();

    programFirst.setUniformValue("znear", pglwidget->camera()->getZnear());
    programFirst.setUniformValue("zfar",  pglwidget->camera()->getZfar());
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    if (pdrawPlugin)
        pdrawPlugin->drawScene();

    programFirst.release();

    GLfloat projmat[16];
    glGetFloatv(GL_PROJECTION_MATRIX, projmat);


    // second pass: compute SSAO factor
    Vector fc[8];
    pglwidget->camera()->getFrustumCornersEyeSpace(fc);

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBuffer2);
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
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, normaldepthTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, rotationPatternTexture);

    programSSAO.setUniformValue("normalsDepth", 0);
    programSSAO.setUniformValue("rotationPattern", 1);
    programSSAO.setUniformValueArray("samplingPattern", samplingPattern, 16);
    programSSAO.setUniformValue("rotationRepetitions", pglwidget->width()/4.0f, pglwidget->height()/4.0f);
    programSSAO.setUniformValue("texelSize", 1.0f/pglwidget->width(), 1.0f/pglwidget->height());
    switch (sampleMode) {
        case WORLD_SPACE:  programSSAO.setUniformValue("radius", 0.25f*sampleRadiusSize*pglwidget->scene()->boundingBox().radius());
                           programSSAO.setUniformValue("zfar",pglwidget->camera()->getZfar());
                           programSSAO.setUniformValue("projectionMatrix",
                                                        QMatrix4x4( projmat[0], projmat[4], projmat[8],  projmat[12],
                                                                    projmat[1], projmat[5], projmat[9],  projmat[13],
                                                                    projmat[2], projmat[6], projmat[10], projmat[14],
                                                                    projmat[3], projmat[7], projmat[11], projmat[15]));
                           break;
        case SCREEN_SPACE: programSSAO.setUniformValue("radius", 32.0f*sampleRadiusSize); break;
        default:           programSSAO.setUniformValue("radius", 1.0f); break;
    }

    glBegin(GL_QUADS);
        glMultiTexCoord2f(GL_TEXTURE0, 0, 0);
        glMultiTexCoord3f(GL_TEXTURE1, fc[4].x(), fc[4].y(), fc[4].z());
        glVertex3f(0, 0, 0);
        glMultiTexCoord2f(GL_TEXTURE0, 1, 0);
        glMultiTexCoord3f(GL_TEXTURE1, fc[5].x(), fc[5].y(), fc[5].z());
        glVertex3f(1, 0, 0);
        glMultiTexCoord2f(GL_TEXTURE0, 1, 1);
        glMultiTexCoord3f(GL_TEXTURE1, fc[6].x(), fc[6].y(), fc[6].z());
        glVertex3f(1, 1, 0);
        glMultiTexCoord2f(GL_TEXTURE0, 0, 1);
        glMultiTexCoord3f(GL_TEXTURE1, fc[7].x(), fc[7].y(), fc[7].z());
        glVertex3f(0, 1, 0);
    glEnd();

    programSSAO.release();


    // third pass: filtering
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

    programFilter.bind();

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
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, aoTexture);

    programFilter.setUniformValue("texture", 0);
    if (filterMode != NO_FILTER) {
        programFilter.setUniformValue("radius", filterRadiusSize);
        programFilter.setUniformValue("texelSize", 1.0f/pglwidget->width(), 1.0f/pglwidget->height());
        if (filterMode == BILATERAL_BLUR) {
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, normaldepthTexture);
            programFilter.setUniformValue("normalsDepth", 1);
            glActiveTexture(GL_TEXTURE0);
        }
    }

    glBegin(GL_QUADS);
        glMultiTexCoord2f(GL_TEXTURE0, 0, 0);   glVertex3f(0, 0, 0);
        glMultiTexCoord2f(GL_TEXTURE0, 1, 0);   glVertex3f(1, 0, 0);
        glMultiTexCoord2f(GL_TEXTURE0, 1, 1);   glVertex3f(1, 1, 0);
        glMultiTexCoord2f(GL_TEXTURE0, 0, 1);   glVertex3f(0, 1, 0);
    glEnd();

    programFilter.release();
}

void RenderSSAO::initGL()
{
    reloadShaders();

    generateSamplingPattern();
    generateRotationPattern();
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);

    if (aoTexture)
        glDeleteTextures(1, &aoTexture);
    glGenTextures(1, &aoTexture);
    glBindTexture(GL_TEXTURE_2D, aoTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    if (depthRenderBuffer)
        glDeleteRenderbuffers(1, &depthRenderBuffer);
    glGenRenderbuffersEXT(1, &depthRenderBuffer);
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthRenderBuffer);
    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, width, height);

    if (frameBuffer1)
        glDeleteFramebuffers(1, &frameBuffer1);
    glGenFramebuffersEXT(1, &frameBuffer1);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBuffer1);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, normaldepthTexture, 0);
    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depthRenderBuffer);
    GLenum status;
    status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    if (status != GL_FRAMEBUFFER_COMPLETE_EXT) {
            std::cerr << "DrawSSAO: Error creating the frame buffer" << std::endl;
    }

    if (frameBuffer2)
        glDeleteFramebuffers(1, &frameBuffer2);
    glGenFramebuffersEXT(1, &frameBuffer2);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBuffer2);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, aoTexture, 0);
    status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    if (status != GL_FRAMEBUFFER_COMPLETE_EXT) {
            std::cerr << "DrawSSAO: Error creating the frame buffer" << std::endl;
    }

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}


void RenderSSAO::generateSamplingPattern() {
    //float samples[16][3];
    for (int i = 0; i < 16; i++) {
        float x = (rand()%1024 - 512)/512.0f;
        float y = (rand()%1024 - 512)/512.0f;
        float z;
        if (sampleMode == WORLD_SPACE)
            z = (rand()%1024 - 512)/512.0f;
        else
            z = 0.0f;
        Vector vec(x, y, z);
        vec.normalize();

        float scale = i/16.0f;
        scale = 0.1 + 0.9*scale*scale;
        vec *= scale;

        /*samples[i][0] = vec.x();
        samples[i][1] = vec.y();
        samples[i][2] = vec.z();*/
        samplingPattern[i] = QVector3D(vec.x(), vec.y(), vec.z());
    }

    /*glGenTextures(1, &samplingPatternTexture);
    glBindTexture(GL_TEXTURE_2D, samplingPatternTexture);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, 16, 0, GL_RGB, GL_FLOAT, samples);*/
}


void RenderSSAO::generateRotationPattern() {
    float rotation[4][4];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            rotation[i][j] = (rand()%360)/360.0f;
        }
    }

    glGenTextures(1, &rotationPatternTexture);
    glBindTexture(GL_TEXTURE_2D, rotationPatternTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, 4, 4, 0, GL_LUMINANCE, GL_FLOAT, &rotation);
}


void RenderSSAO::setSampleMode(SampleMode sm) {
    sampleMode = sm;
    generateSamplingPattern();
}

void RenderSSAO::setSampleRadiusSize(float r) {
    sampleRadiusSize = r;
}

void RenderSSAO::setFilterRadiusSize(float r) {
    filterRadiusSize = 5.0f*r;
}

void RenderSSAO::setFilterMode(FilterMode fm) {
    filterMode = fm;
}

void RenderSSAO::reloadShaders() {
    std::cerr << "DrawSSAO: initializing shaders..." << std::endl;

    std::cerr << "    ...first pass" << std::endl;
    programFirst.removeAllShaders();
    programFirst.addShaderFromSourceFile(QGLShader::Vertex, "../../shaders/firstPass.vert");
    programFirst.addShaderFromSourceFile(QGLShader::Fragment, "../../shaders/firstPass.frag");
    if (!programFirst.link()) {
        std::cerr << "DrawSSAO: error initializing firstPass shader program" << endl;
        std::cerr << programFirst.log().toStdString() << std::endl;
    }

    std::cerr << "    ...ssao pass (" << sampleMode << ")" << std::endl;
    programSSAO.removeAllShaders();
    switch (sampleMode) {
        case WORLD_SPACE:
            programSSAO.addShaderFromSourceFile(QGLShader::Vertex, "../../shaders/ssaoWS.vert");
            programSSAO.addShaderFromSourceFile(QGLShader::Fragment, "../../shaders/ssaoWS.frag");
            break;
        case SCREEN_SPACE:
            programSSAO.addShaderFromSourceFile(QGLShader::Vertex, "../../shaders/ssaoSS.vert");
            programSSAO.addShaderFromSourceFile(QGLShader::Fragment, "../../shaders/ssaoSS.frag");
            break;
        default:
            break;
    }
    if (!programSSAO.link()) {
        std::cerr << "DrawSSAO: error initializing programSSAO shader program" << endl;
        std::cerr << programSSAO.log().toStdString() << std::endl;
    }

    std::cerr << "    ...filter pass (" << filterMode << ")" << std::endl;
    programFilter.removeAllShaders();
    switch (filterMode) {
        case NO_FILTER:
            programFilter.addShaderFromSourceFile(QGLShader::Vertex, "../../shaders/filter.vert");
            programFilter.addShaderFromSourceFile(QGLShader::Fragment, "../../shaders/filterNone.frag");
            break;
        case GAUSSIAN_BLUR:
            programFilter.addShaderFromSourceFile(QGLShader::Vertex, "../../shaders/filter.vert");
            programFilter.addShaderFromSourceFile(QGLShader::Fragment, "../../shaders/filterGauss.frag");
            break;
        case BILATERAL_BLUR:
            programFilter.addShaderFromSourceFile(QGLShader::Vertex, "../../shaders/filter.vert");
            programFilter.addShaderFromSourceFile(QGLShader::Fragment, "../../shaders/filterBilateral.frag");
            break;
        default:
            break;
    }
    if (!programFilter.link()) {
        std::cerr << "DrawSSAO: error initializing programFilter shader program" << endl;
        std::cerr << programFilter.log().toStdString() << std::endl;
    }

}


Q_EXPORT_PLUGIN2(renderssao, RenderSSAO)   // plugin name, plugin class
