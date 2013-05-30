#include "drawvbo.h"
#include "glwidget.h"
#include "object.h"
#include "box.h"
#include <queue>
#include <utility>


void DrawVBO::initGL()
{
}

void DrawVBO::drawScene()
{
    glColor3f(0.8f, 0.8f, 0.8f);
    for (unsigned int i = 0; i < vbo.size(); i++)
        drawObjectVBO(i);
}


void DrawVBO::drawObjectVBO(int i) {
    glBindBufferARB(GL_ARRAY_BUFFER, vbo[i]);
    glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, ibo[i]);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    glVertexPointer(3, GL_FLOAT, 6*sizeof(float), 0);
    glNormalPointer(   GL_FLOAT, 6*sizeof(float), (void*)(3*sizeof(float)));

    glDrawElements(GL_TRIANGLES, drawsize[i], GL_UNSIGNED_INT, 0);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}



void DrawVBO::sendObjectToGPU(Object& obj) {

    const std::vector<Vertex>& pvertices = obj.vertices();
    const std::vector<Face>& pfaces = obj.faces();

    const int VERTEX_SIZE = 6;
    GLfloat data[VERTEX_SIZE*pvertices.size()];
    for (unsigned int i = 0; i < pvertices.size(); i++) {
        const Vertex& v = pvertices[i];
        data[VERTEX_SIZE*i + 0] = v.coord().x();
        data[VERTEX_SIZE*i + 1] = v.coord().y();
        data[VERTEX_SIZE*i + 2] = v.coord().z();
        data[VERTEX_SIZE*i + 3] = v.normal().x();
        data[VERTEX_SIZE*i + 4] = v.normal().y();
        data[VERTEX_SIZE*i + 5] = v.normal().z();
    }
    GLuint index[3*pfaces.size()];
    for (unsigned int i = 0; i < pfaces.size(); i++) {
        const Face& f = pfaces[i];
        index[3*i + 0] = f.vertexIndex(0);
        index[3*i + 1] = f.vertexIndex(1);
        index[3*i + 2] = f.vertexIndex(2);
    }

    GLuint pVBO, pIBO;
    glGenBuffersARB(1, &pVBO);
    glBindBufferARB(GL_ARRAY_BUFFER, pVBO);
    glBufferDataARB(GL_ARRAY_BUFFER, VERTEX_SIZE*sizeof(GLfloat)*pvertices.size(), data, GL_STATIC_DRAW);
    glGenBuffersARB(1, &pIBO);
    glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, pIBO);
    glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER, 3*sizeof(GLuint)*pfaces.size(), index, GL_STATIC_DRAW);

    glBindBufferARB(GL_ARRAY_BUFFER, 0);
    glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, 0);

    vbo.push_back(pVBO);
    ibo.push_back(pIBO);
    drawsize.push_back(3*pfaces.size());
}


void DrawVBO::onObjectAdd()
{
    int nobjs = pglwidget->scene()->objects().size();
    vbo.clear();
    vbo.reserve(nobjs);
    ibo.clear();
    ibo.reserve(nobjs);
    for (int i = 0; i < nobjs; i++) {
        Object obj = pglwidget->scene()->objects()[i].toTriangles();
        obj.computeVertexNormalsPerFace();
        sendObjectToGPU(obj);
    }
}


Q_EXPORT_PLUGIN2(drawvbo, DrawVBO)   // plugin name, plugin class
