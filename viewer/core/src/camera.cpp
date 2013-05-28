#include "camera.h"
#include <cmath>
#include <QMatrix4x4>



void Camera::init(const Box& box)
{
    Point center = box.center();
    float radius = box.radius();

    pvrp = center;
    pdist = 2*radius;
    pfovy = 60; // 2 * asin (radi/2*radi)
    pzNear = radius;
    pzFar = 3*radius;

    pangleX = 0;
    pangleY = 0;
    pangleZ = 0;
    setProjection();
    setModelview();
}

void Camera::setModelview() const
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0,0,-pdist);
    glRotatef(-pangleZ,0,0,1);
    glRotatef( pangleX,1,0,0);
    glRotatef(-pangleY,0,1,0);
    glTranslatef(-pvrp.x(),-pvrp.y(),-pvrp.z());
}

void Camera::setProjection() const
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float realFovy = pfovy;
    if (paspectRatio < 1.0) 
      realFovy = 360.0/M_PI*atan(tan(pfovy*M_PI/360)/paspectRatio);
    gluPerspective(realFovy, paspectRatio, pzNear, pzFar);
}

Point Camera::getObs() const
{
    double viewMatrix[16];
    glGetDoublev(GL_MODELVIEW_MATRIX, viewMatrix);

    QMatrix4x4 m = QMatrix4x4(viewMatrix).transposed(); // transposta perquè el constructor de QMatrix4x4 assumeix data en row-major order
    QMatrix4x4 inv = m.inverted(); 
    return QVector3D(inv*QVector4D(0.0, 0.0, 0.0, 1.0));
}

void Camera::getAxes(Vector& camx, Vector& camy, Vector& camz) {
    setModelview();
    double viewMatrix[16];
    glGetDoublev(GL_MODELVIEW_MATRIX, viewMatrix);
    camx = Vector(viewMatrix[0], viewMatrix[4], viewMatrix[8]);
    camy = Vector(viewMatrix[1], viewMatrix[5], viewMatrix[9]);
    camz = Vector(viewMatrix[2], viewMatrix[6], viewMatrix[10]);
}

void Camera::getAngles(float &ax, float &ay, float &az) const
{
    ax = pangleX;
    ay = pangleY;
    az = pangleZ;
}

void Camera::setAngles(float ax, float ay, float az)
{
    pangleX = ax;
    pangleY = ay;
    pangleZ = az;
}

void Camera::setAspectRatio(float ar)
{
    paspectRatio = ar;
    setProjection();
}

void Camera::incrementDistance(float inc)
{
    pdist += inc;
    setModelview();
}

void Camera::incrementAngleX(float inc)
{
    pangleX += inc;
    setModelview();
}

void Camera::incrementAngleY(float inc)
{
    pangleY += inc;
    setModelview();
}

void Camera::pan(const Vector& offset)
{
	pvrp = pvrp + offset;
	setModelview();
}

void Camera::moveLocal(const Vector& offset)
{
    Vector cx, cy, cz;
    this->getAxes(cx, cy, cz);
    Vector dir = offset.x()*cx + offset.y()*cy + offset.z()*cz;
    pvrp += dir;
    setModelview();
}

void Camera::rotateLocal(const Vector& angles)
{
    Vector obs = getObs();
    QMatrix4x4 trans;
    trans.setToIdentity();
    trans.translate( obs.x(),  obs.y(),  obs.z());
    trans.rotate(angles.y(), 0, 1, 0);
    trans.rotate(angles.x(), 1, 0, 0);
    trans.rotate(angles.z(), 0, 0, 1);
    trans.translate(-obs.x(), -obs.y(), -obs.z());

    pvrp = trans*pvrp;
    pangleX += angles.x();
    pangleY += angles.y();
    pangleZ += angles.z();
    setModelview();
}

void Camera::updateClippingPlanes(const Box& box)
{
    if (box.radius() < 0.001f) return;

    Point obs = getObs();
    Vector v = pvrp - obs;
    v.normalize();
    Vector u = box.center()-obs;
    pzNear = Vector::dotProduct(u,v)-box.radius();
    if (pzNear < 0.1f) pzNear = 0.1f;
    pzFar = Vector::dotProduct(u,v) + box.radius();
    setProjection();
}

Frustum Camera::getFrustum() const {
    Vector zdir = getObs() - pvrp;
    zdir.normalize();
	Vector sup = abs(zdir.y() > 0.95) ? Vector(0, 0, 1) : Vector(0, 1, 0);
	Vector xdir = sup ^ zdir;
	xdir.normalize();
    Vector vup = zdir ^ xdir;
	vup.normalize();

    Frustum f;
    f.setParams(getObs(), pvrp, vup, paspectRatio, pfovy, pzNear, pzFar);
    return f;
}

void Camera::getFrustumCorners(Vector corners[8]) const {

    // Calculem el sistema de referencia de la camera
    Vector obs  = Vector(getObs());
    Vector zCam = Vector(getObs()) - Vector(pvrp);
    zCam.normalize();
    Vector sup = abs(zCam.y() > 0.95) ? Vector(0, 0, 1) : Vector(0, 1, 0);
    Vector xCam = sup ^ zCam;
    xCam.normalize();
    Vector yCam = zCam ^ xCam;
    yCam.normalize();

    // Calculem l'alcada i l'amplada dels plans de retallat
    float hNear = 2 * tan(DEG2RAD*pfovy/2) * pzNear;
    float hFar  = 2 * tan(DEG2RAD*pfovy/2) * pzFar;
    float wNear = hNear * paspectRatio;
    float wFar  = hFar  * paspectRatio;

    // Calculem el centre del pla i els extrems
    Point nc  = obs - zCam * pzNear;
    Point fc  = obs - zCam * pzFar;
    Point ftl = fc + (yCam * hFar/2)  - (xCam * wFar/2);
    Point ftr = fc + (yCam * hFar/2)  + (xCam * wFar/2);
    Point fbl = fc - (yCam * hFar/2)  - (xCam * wFar/2);
    Point fbr = fc - (yCam * hFar/2)  + (xCam * wFar/2);
    Point ntl = nc + (yCam * hNear/2) - (xCam * wNear/2);
    Point ntr = nc + (yCam * hNear/2) + (xCam * wNear/2);
    Point nbl = nc - (yCam * hNear/2) - (xCam * wNear/2);
    Point nbr = nc - (yCam * hNear/2) + (xCam * wNear/2);

    corners[0] = nbl;
    corners[1] = nbr;
    corners[2] = ntr;
    corners[3] = ntl;
    corners[4] = fbl;
    corners[5] = fbr;
    corners[6] = ftr;
    corners[7] = ftl;

}


void Camera::getFrustumCornersEyeSpace(Vector corners[8]) const {

    float hNear = 2 * tan(DEG2RAD*pfovy/2) * pzNear;
    float hFar  = 2 * tan(DEG2RAD*pfovy/2) * pzFar;
    float wNear = hNear * paspectRatio;
    float wFar  = hFar  * paspectRatio;

    Point ftl = Point(-wFar /2,  hFar /2,  pzFar);
    Point ftr = Point( wFar /2,  hFar /2,  pzFar);
    Point fbl = Point(-wFar /2, -hFar /2,  pzFar);
    Point fbr = Point( wFar /2, -hFar /2,  pzFar);
    Point ntl = Point(-wNear/2,  hNear/2, pzNear);
    Point ntr = Point( wNear/2,  hNear/2, pzNear);
    Point nbl = Point(-wNear/2, -hNear/2, pzNear);
    Point nbr = Point( wNear/2, -hNear/2, pzNear);

    corners[0] = nbl;
    corners[1] = nbr;
    corners[2] = ntr;
    corners[3] = ntl;
    corners[4] = fbl;
    corners[5] = fbr;
    corners[6] = ftr;
    corners[7] = ftl;

}
