#ifndef _DRAWVBO_H
#define _DRAWVBO_H

#include "drawinterface.h"
#include "GL/glew.h"
#include "object.h"
#include <vector>

 class DrawVBO : public QObject, public DrawInterface
 {
     Q_OBJECT
     Q_INTERFACES(DrawInterface)

 public:

     virtual void initGL();
     virtual void drawScene();

     void onObjectAdd();

 private:

     void sendObjectToGPU(Object& obj);
     void drawObjectVBO(int index);

     std::vector<GLuint> vbo;
     std::vector<GLuint> ibo;
     std::vector<GLuint> drawsize;

 };
 

 #endif
 
