#ifndef _DRAWOCCLUSIONQUERY_H
#define _DRAWOCCLUSIONQUERY_H

#include "drawinterface.h"
#include "GL/glew.h"
#include "object.h"
#include "drawocclusionwidget.h"
#include <vector>

 class DrawOcclusionQuery : public QObject, public DrawInterface
 {
     Q_OBJECT
     Q_INTERFACES(DrawInterface)

 public:

     DrawOcclusionQuery();

     enum Visibility { VISIBLE, CULLED_FRUSTUM, CULLED_OCCLUSION, DRAWN_DEFAULT };

     virtual void initGL();
     virtual void drawScene();

     void onObjectAdd();

     virtual QWidget* getPluginWidget();
     virtual QString  getPluginName() { return "Occlusion"; }

 public slots:

     void setFrustumCulling(int state);
     void setOcclusionQueries(int state);
     void setFullGeometry(int state);
     void setDrawInfo(int state);

 private:

     static const unsigned int MIN_PIXEL_COUNT = 1;

     void sendObjectToGPU(Object& obj);
     void drawObjectVBO(int index);

     std::vector<GLuint> vbo;
     std::vector<GLuint> ibo;
     std::vector<GLuint> drawsize;

     bool doFrustumCulling;
     bool doOcclusionQueries;
     bool useFullGeometry;
     bool drawInfo;

     GLuint infoTexture;
     GLuint frameBuffer;

     DrawOcclusionWidget* widget;

 };
 

 #endif
 
