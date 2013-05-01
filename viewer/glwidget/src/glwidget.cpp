#define _USE_MATH_DEFINES 1
#include <cmath>
#include "glwidget.h"
#include <QMatrix4x4>
#include <GL/glu.h>
#include <QPluginLoader>
#include <QCoreApplication>
#include <QTime>

const float DRAW_AXES_LENGTH = 20.0;

GLWidget::GLWidget(QWidget * parent): QGLWidget(parent)
{
    drawPlugin = 0;
    renderPlugin = 0;
    frames = 0;
    benchmarking = false;
    initialized = false;
    setFocusPolicy(Qt::ClickFocus);  // per rebre events de teclat
}

void GLWidget::initDefaultPlugins() {
    loadPlugin("../../plugins/bin/libdrawvbo.so");
    loadPlugin("../../plugins/bin/librenderssao.so");
    loadPlugin("../../plugins/bin/libnavigatedefault.so");
    loadPlugin("../../plugins/bin/libnavigatewasd.so");
}

void GLWidget::initializeGL()
{
    if (glewInit() != GLEW_OK) { // necessari per cridar funcions de les darreres versions d'OpenGL
        std::cerr << "Error initializing GLEW!" << std::endl;
    }
    glClearColor(0.8f, 0.8f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING); 
    glEnable(GL_LIGHT0);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

	if (drawPlugin)		drawPlugin->initGL();
	if (renderPlugin)	renderPlugin->initGL();
	for (unsigned int i = 0; i < effectPlugins.size(); i++)
		effectPlugins[i]->initGL();

    t0 = QTime::currentTime();
    tprev = t0;
    connect(&timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    timer.start(1);

    initialized = true;
}

Scene* GLWidget::scene()
{
    return &pscene;
}

Camera* GLWidget::camera()
{
    return &pcamera;
}


void GLWidget::drawAxes() const
{
    float L = DRAW_AXES_LENGTH;
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
    glColor3f(1,0,0); glVertex3f(0,0,0); glVertex3f(L,0,0); // X
    glColor3f(0,1,0); glVertex3f(0,0,0); glVertex3f(0,L,0); // Y
    glColor3f(0,0,1); glVertex3f(0,0,0); glVertex3f(0,0,L); // Z
    glEnd();
    glEnable(GL_LIGHTING);
}

Box GLWidget::boundingBoxIncludingAxes() const
{
    float L = DRAW_AXES_LENGTH;
    Box box(Point(0,0,0), Point(L,L,L)); // AABB dels eixos
    if (pscene.objects().size())
        box.expand(pscene.boundingBox());
    return box;
}

void GLWidget::resetCamera()
{
    pcamera.init(boundingBoxIncludingAxes());
    updateGL();
}

void GLWidget::paintGL( void )
{ 
    timer.stop();

    // logic
    QTime tnow = QTime::currentTime();
    float dt = tprev.msecsTo(tnow)/1000.0f;
    tprev = tnow;
    for (unsigned int i = 0; i < actionPlugins.size(); i++)
        actionPlugins[i]->update(dt);

    // render
    for (unsigned int i=0; i<effectPlugins.size(); ++i) 
        effectPlugins[i]->preFrame();
    
    if (renderPlugin) renderPlugin->paintGL();

    for (unsigned int i=0; i<effectPlugins.size(); ++i) 
        effectPlugins[i]->postFrame();

    // FPS counter
    frames++;
    if (frames >= 200) {
        glFinish();

        QTime t1 = QTime::currentTime();
        int msecs = t0.msecsTo(t1);
        float fps = 200*1000.0f/msecs;

        if (benchmarking) {
            times.push_back(fps);
        }

        emit updatedFPS(fps);

        frames = 0;
        t0 = QTime::currentTime();
    }

    timer.start(0);
}

void GLWidget::resizeGL (int width, int height)
{
    glViewport (0, 0, width, height);
    pcamera.setAspectRatio( (float)width/(float)height);

    if (renderPlugin)
        renderPlugin->resizeGL(width, height);
}

void GLWidget::startBenchmarking()
{
    benchmarking = true;
    frames = 0;
    times.clear();
    times.reserve(1000);
}

void GLWidget::stopBenchmarking()
{
    timer.stop();

    benchmarking = false;

    QFile file("benchmark.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    for (unsigned int i = 0; i < times.size(); i++) {
        out << times[i] << "\n";
    }
    file.close();

    timer.start(0);
}

void GLWidget::loadPlugin(const QString& name) {
    QPluginLoader loader(name);
    loader.load();
    if (loader.isLoaded())
    {
        //cout << "Loaded!" << endl;
        QObject *plugin = loader.instance();
        if (plugin)
        {
            //qDebug() << "instance OK" << name << endl;
            // determinar tipus d'interface
            DrawInterface * tmpDraw = qobject_cast<DrawInterface *>(plugin);
            if (tmpDraw)
            {
                qDebug() << "Loaded draw plugin: " << name << endl;
                drawPlugin = tmpDraw;
                drawPlugin->setWidget(this);
                if (renderPlugin) renderPlugin->setDrawPlugin(drawPlugin);
                drawPlugin->onPluginLoad();
                if (initialized) drawPlugin->initGL();
                resetCamera();
            }

            RenderInterface * tmpRender = qobject_cast<RenderInterface *>(plugin);
            if (tmpRender)
            {
                qDebug() << "Loaded render plugin: " << name << endl;
                renderPlugin = tmpRender;
                renderPlugin->setWidget(this);
                if (drawPlugin) renderPlugin->setDrawPlugin(drawPlugin);
                renderPlugin->onPluginLoad();
                if (initialized) renderPlugin->initGL();
                resetCamera();
            }

            EffectInterface * tmpEffect = qobject_cast<EffectInterface *>(plugin);
            if (tmpEffect)
            {
                qDebug() << "Added effect plugin: " << name << endl;
                tmpEffect->setWidget(this);
                tmpEffect->onPluginLoad();
                effectPlugins.push_back(tmpEffect);
            }

            ActionInterface * tmpAction = qobject_cast<ActionInterface *>(plugin);
            if (tmpAction)
            {
                qDebug() << "Added action plugin: " << name << endl;
                tmpAction->setWidget(this);
                tmpAction->onPluginLoad();
                actionPlugins.push_back(tmpAction);
                resetCamera();
            }

            GenericPluginInterface* gplugin = dynamic_cast<GenericPluginInterface*>(plugin);
            if (gplugin->getPluginWidget()) {
                emit addedPluginWidget(gplugin->getPluginWidget(), gplugin->getPluginName());
            }
        }
    }
    else
    {
        qDebug() << "Load error: " << name << endl;
        qDebug() << loader.errorString() << endl;
    }
}


void GLWidget::loadPlugin()
{
    // TO DO: canviar extensio segons OS
    QStringList list = QFileDialog::getOpenFileNames(NULL, "Select one or more plugins to open", "../../plugins/bin", "Plugins (*.dll *.so *.dylib)");
    QStringList::Iterator it = list.begin();
    while(it != list.end()) 
    {
        QString name = *it;
        loadPlugin(name);
        ++it;
    }
    updateGL();
}

void GLWidget::help( void ){
    cout<<"Tecles definides: \n";
    cout<<"P         Afegeix plugins\n";
    cout<<"L         Afegeix un objecte\n";
    cout<<"F         Pinta en filferros\n";
    cout<<"M         Pinta amb omplert de polígons\n";
    cout<<"H         Imprimir aquesta ajuda\n";
}

void GLWidget::keyPressEvent(QKeyEvent *e)
{
    switch( e->key() ) 
    {	    
    case Qt::Key_P:
        loadPlugin();
        updateGL();
        break;

    case Qt::Key_L: 
        addObject();
        break;        
        
    case Qt::Key_F: 
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        updateGL();
        break;

    case Qt::Key_M:
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        updateGL();
        break;

    case Qt::Key_H:
        help();
        break;
        
    default: 
        e->ignore(); // el propaguem cap al pare...
    }
    
    for (unsigned int i=0; i<actionPlugins.size(); ++i) 
        actionPlugins[i]->keyPressEvent(e);
    
}


void GLWidget::mousePressEvent( QMouseEvent *e)
{
    for (unsigned int i=0; i<actionPlugins.size(); ++i) 
        actionPlugins[i]->mousePressEvent(e);
}

void GLWidget::mouseReleaseEvent( QMouseEvent *e)
{
    for (unsigned int i=0; i<actionPlugins.size(); ++i) 
        actionPlugins[i]->mouseReleaseEvent(e);
}

void GLWidget::mouseMoveEvent(QMouseEvent *e)
{ 
    for (unsigned int i=0; i<actionPlugins.size(); ++i) 
        actionPlugins[i]->mouseMoveEvent(e);
}

void	 GLWidget::keyReleaseEvent ( QKeyEvent * e)
{
    for (unsigned int i=0; i<actionPlugins.size(); ++i) 
        actionPlugins[i]->keyReleaseEvent(e);
}

void	 GLWidget::wheelEvent ( QWheelEvent *e)
{
    for (unsigned int i=0; i<actionPlugins.size(); ++i) 
        actionPlugins[i]->wheelEvent(e);
}

void GLWidget::addObjectFromFile(const QString& filename)
{
    std::string name = filename.toLocal8Bit().constData();
    Object o(name.c_str());
    o.readObj(name.c_str());
    pscene.addObject(o);
    
    // propagate to all plugins
    if (drawPlugin) drawPlugin->onObjectAdd();
    if (renderPlugin) renderPlugin->onObjectAdd();
    for (unsigned int i=0; i<actionPlugins.size(); ++i) 
        actionPlugins[i]->onObjectAdd();
    for (unsigned int i=0; i<effectPlugins.size(); ++i) 
        effectPlugins[i]->onObjectAdd();
    
    resetCamera();
    updateGL();
}

void GLWidget::addObject()
{
    QStringList files = QFileDialog::getOpenFileNames(NULL, "Select one or more models to open", "", "Models (*.obj)");
    QStringList::Iterator it = files.begin();
    while(it != files.end()) 
    {
        QString    fileName = (*it);
        addObjectFromFile(fileName);
        ++it;
    }
}

