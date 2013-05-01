#include "drawocclusion.h"
#include "glwidget.h"
#include "object.h"
#include "box.h"
#include <queue>
#include <utility>

DrawOcclusionQuery::DrawOcclusionQuery()
{
    widget = new DrawOcclusionWidget();
}

QWidget* DrawOcclusionQuery::getPluginWidget()
{
    return widget;
}

void DrawOcclusionQuery::initGL()
{
    glGenTextures(1, &infoTexture);
    glBindTexture(GL_TEXTURE_2D, infoTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 256, 256, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);

    glGenFramebuffersEXT(1, &frameBuffer);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBuffer);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, infoTexture, 0);

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

    GLenum status;
    status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    if (status != GL_FRAMEBUFFER_COMPLETE_EXT) {
            std::cerr << "DrawOcclusionQuery: Error creating the frame buffer" << std::endl;
    }
}


void DrawOcclusionQuery::drawScene()
{
    glColor3f(0.8f, 0.8f, 0.8f);

    Camera* cam = pglwidget->camera();
    Point obs = cam->getObs();
    Frustum frustum = cam->getFrustum();
    Scene* scene = pglwidget->scene();
    const std::vector<Object>& objs = scene->objects();

    std::vector<Visibility> objState(objs.size(), VISIBLE);
    std::priority_queue<std::pair<float, int> > sortedObjs;

    for (unsigned int i = 0; i < vbo.size(); i++) {
        // frustum culling
        if (widget->doFrustumCulling() && !frustum.intersects(objs[i].boundingBox())) {
            objState[i] = CULLED_FRUSTUM;
        }
        // if inside frustum and occlusion queries enabled, put it in pqueue
        else if (widget->doOcclusionQueries()) {
            float dist = Vector(obs, objs[i].boundingBox().center()).lengthSquared();
            sortedObjs.push(std::pair<float, int>(-dist, i));
        }
        // otherwise, just draw it
        else {
            drawObjectVBO(i);
        }
    }

    // if we are doing occlusion queries
    if (widget->doOcclusionQueries()) {

        const unsigned int layers = 4;
        const unsigned int fixedObjs = 16;
        int objPerLayer = (objs.size() - fixedObjs)/layers + int((objs.size() - fixedObjs)%layers > 0);
        std::vector<GLuint> queries(objPerLayer, 0);
        std::vector<int>    objIds(objPerLayer, -1);

        // render first set of objects, considered as visible occluders
        for (unsigned int i = 0; i < fixedObjs && sortedObjs.size() > 0; i++) {
            std::pair<float, int> p = sortedObjs.top();
            sortedObjs.pop();
            drawObjectVBO(p.second);
            objState[p.second] = DRAWN_DEFAULT;
        }

        // for each other layer, send some queries and wait for results
        for (unsigned int dl = 0; dl < layers && sortedObjs.size(); dl++) {

            // disable rendering state
            glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
            glDepthMask(GL_FALSE);
            glDisable(GL_LIGHTING);

            // generate and send the queries
            int queriesSent = 0;
            glGenQueries(objPerLayer, &queries[0]);
            for (int i = 0; i < objPerLayer && sortedObjs.size() > 0; i++) {
                int id = sortedObjs.top().second;
                objIds[i] = id;
                sortedObjs.pop();

                glBeginQuery(GL_ANY_SAMPLES_PASSED, queries[i]);
                if (widget->useRealGeometry()) {
                    drawObjectVBO(id);
                }
                else {
                    objs[id].boundingBox().render(false);
                }
                glEndQuery(GL_ANY_SAMPLES_PASSED);

                queriesSent++;
            }

            // enable again relevant rendering state
            glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
            glDepthMask(GL_TRUE);
            glEnable(GL_LIGHTING);

            // collect each query result
            for (int i = 0; i < queriesSent; i++) {

                // if we have a pending query, get the result
                GLuint pixelCount;
                glGetOcclusionQueryuivNV(queries[i], GL_QUERY_RESULT, &pixelCount);

                // occlusion culling
                if (pixelCount < MIN_PIXEL_COUNT) {
                    objState[objIds[i]] = CULLED_OCCLUSION;
                }
                // object still visible, draw it
                else {
                    drawObjectVBO(objIds[i]);
                }
            }

            // delete the queries
            glDeleteQueries(objPerLayer, &queries[0]);
        }
    }

    // now draw a minimap with the result for a better visibility
    if (widget->drawDebugInfo()) {
        float r = scene->boundingBox().radius();
        Point c = scene->boundingBox().center();

        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBuffer);
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glViewport(0, 0, 256, 256);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-r, r, -r, r, -r, r);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glRotatef(90, 1, 0, 0);
        glTranslatef(-c.x(), -c.y() + r, -c.z());

        glDisable(GL_LIGHTING);

        for (unsigned int i = 0; i < objs.size(); i++) {
            switch (objState[i]) {
                case CULLED_FRUSTUM:	glColor3f(1, 0, 0); break;
                case CULLED_OCCLUSION:	glColor3f(0, 0, 1); break;
                case VISIBLE:		glColor3f(0, 1, 0); break;
                case DRAWN_DEFAULT:     glColor3f(1, 1, 0); break;
                default:		glColor3f(1, 0, 1); break;
            }
            objs[i].boundingBox().render(false);
        }

        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

        glViewport(0, 0, pglwidget->width(), pglwidget->height());
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(0, 1, 0, 1, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1, 1, 1, 0.5f);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, infoTexture);

        float s = 0.2f / cam->getAspectRatio();
        float x = 0.01f / cam->getAspectRatio();
        glBegin(GL_QUADS);
            glTexCoord2f(0, 0); glVertex3f(x,     0.79f, 0);
            glTexCoord2f(1, 0); glVertex3f(x + s, 0.79f, 0);
            glTexCoord2f(1, 1); glVertex3f(x + s, 0.99f, 0);
            glTexCoord2f(0, 1); glVertex3f(x,     0.99f, 0);
        glEnd();

        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();

    }

}


void DrawOcclusionQuery::drawObjectVBO(int i) {
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



void DrawOcclusionQuery::sendObjectToGPU(Object& obj) {

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


void DrawOcclusionQuery::onObjectAdd()
{
    int nobjs = pglwidget->scene()->objects().size();
    pglwidget->scene()->replicateObject(nobjs-1, 8, 1, 8);
    nobjs = pglwidget->scene()->objects().size();

    vbo.clear();
    vbo.reserve(nobjs);
    ibo.clear();
    ibo.reserve(nobjs);
    for (int i = 0; i < nobjs; i++) {
        Object obj = pglwidget->scene()->objects()[i].toTriangles();
        obj.computeVertexNormals();
        sendObjectToGPU(obj);
    }
}

void DrawOcclusionQuery::setFrustumCulling(int state) {
    if (state == Qt::Checked) doFrustumCulling = true;
    else                      doFrustumCulling = false;
}

void DrawOcclusionQuery::setOcclusionQueries(int state) {
    if (state == Qt::Checked) doOcclusionQueries = true;
    else                      doOcclusionQueries = false;
}

void DrawOcclusionQuery::setFullGeometry(int state) {
    if (state == Qt::Checked) useFullGeometry = true;
    else                      useFullGeometry = false;
}

void DrawOcclusionQuery::setDrawInfo(int state) {
    if (state == Qt::Checked) drawInfo = true;
    else                      drawInfo = false;
}

Q_EXPORT_PLUGIN2(drawocclusion, DrawOcclusionQuery)   // plugin name, plugin class
