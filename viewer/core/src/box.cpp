#include "box.h"
#include <QtOpenGL/qgl.h>
#include "vector.h"

Box::Box(const Point& point)
    : pmin(point), pmax(point)
{}

Box::Box(const Point& minimum, const Point& maximum)
    : pmin(minimum), pmax(maximum)
{}

void Box::expand(const Point& p)
{
    if (p.x() < pmin.x()) pmin.setX(p.x());
    if (p.y() < pmin.y()) pmin.setY(p.y());
    if (p.z() < pmin.z()) pmin.setZ(p.z());
    if (p.x() > pmax.x()) pmax.setX(p.x());
    if (p.y() > pmax.y()) pmax.setY(p.y());
    if (p.z() > pmax.z()) pmax.setZ(p.z());
}

void Box::expand(const Box& b)
{
    expand(b.pmin);
    expand(b.pmax);
}

Point Box::center() const
{
    return 0.5f*(pmax+pmin);
}

float Box::radius() const
{
    Vector v = pmax - pmin;
    return 0.5f*v.length();
}

void Box::getVertexs(Point vxs[8]) const {
    vxs[0] = pmin;
    vxs[1] = Point(pmin.x(), pmin.y(), pmax.z());
    vxs[2] = Point(pmax.x(), pmin.y(), pmin.z());
    vxs[3] = Point(pmax.x(), pmin.y(), pmax.z());
    vxs[4] = Point(pmin.x(), pmax.y(), pmin.z());
    vxs[5] = Point(pmin.x(), pmax.y(), pmax.z());
    vxs[6] = Point(pmax.x(), pmax.y(), pmin.z());
    vxs[7] = Point(pmax.x(), pmax.y(), pmax.z());
}

Point Box::getPositiveVertex(const Vector& n) const {
    Point p(pmin);
    if (n.x() >= 0) p.setX(pmax.x());
    if (n.y() >= 0) p.setY(pmax.y());
    if (n.z() >= 0) p.setZ(pmax.z());
    return p;
}

Point Box::getNegativeVertex(const Vector& n) const {
    Point p(pmin);
    if (n.x() < 0) p.setX(pmax.x());
    if (n.y() < 0) p.setY(pmax.y());
    if (n.z() < 0) p.setZ(pmax.z());
    return p;
}


bool Box::contains(const Box& box) const {
    if (!this->contains(box.pmin)) return false;
    if (!this->contains(box.pmax)) return false;
    return true;
}


bool Box::contains(const Point& p) const {
    if (p.x() <  pmin.x()) return false;
    if (p.x() >= pmax.x()) return false;
    if (p.y() <  pmin.y()) return false;
    if (p.y() >= pmax.y()) return false;
    if (p.z() <  pmin.z()) return false;
    if (p.z() >= pmax.z()) return false;
    return true;
}


bool Box::intersects(const Box& box) const {
    Point Amin = pmin;
    Point Amax = pmax;
    Point Bmin = box.pmin;
    Point Bmax = box.pmax;
    if (Amax.x() < Bmin.x()) return false;
    if (Amax.y() < Bmin.y()) return false;
    if (Amax.z() < Bmin.z()) return false;
    if (Amin.x() > Bmax.x()) return false;
    if (Amin.y() > Bmax.y()) return false;
    if (Amin.z() > Bmax.z()) return false;
    return true;
}


void Box::render(bool wire)
{
    if (wire) {
        glBegin(GL_LINES);
        glVertex3f(pmin.x(), pmin.y(), pmin.z());
        glVertex3f(pmax.x(), pmin.y(), pmin.z());

        glVertex3f(pmax.x(), pmin.y(), pmin.z());
        glVertex3f(pmax.x(), pmax.y(), pmin.z());

        glVertex3f(pmax.x(), pmax.y(), pmin.z());
        glVertex3f(pmin.x(), pmax.y(), pmin.z());

        glVertex3f(pmin.x(), pmax.y(), pmin.z());
        glVertex3f(pmin.x(), pmin.y(), pmin.z());

        glVertex3f(pmin.x(), pmin.y(), pmax.z());
        glVertex3f(pmax.x(), pmin.y(), pmax.z());

        glVertex3f(pmax.x(), pmin.y(), pmax.z());
        glVertex3f(pmax.x(), pmax.y(), pmax.z());

        glVertex3f(pmax.x(), pmax.y(), pmax.z());
        glVertex3f(pmin.x(), pmax.y(), pmax.z());

        glVertex3f(pmin.x(), pmax.y(), pmax.z());
        glVertex3f(pmin.x(), pmin.y(), pmax.z());

        glVertex3f(pmin.x(), pmax.y(), pmin.z());
        glVertex3f(pmin.x(), pmax.y(), pmax.z());

        glVertex3f(pmax.x(), pmax.y(), pmin.z());
        glVertex3f(pmax.x(), pmax.y(), pmax.z());

        glVertex3f(pmin.x(), pmin.y(), pmin.z());
        glVertex3f(pmin.x(), pmin.y(), pmax.z());

        glVertex3f(pmax.x(), pmin.y(), pmin.z());
        glVertex3f(pmax.x(), pmin.y(), pmax.z());

        glEnd();
    }
    else {
        glBegin(GL_QUADS);

            glVertex3f(pmin.x(), pmin.y(), pmax.z());
            glVertex3f(pmax.x(), pmin.y(), pmax.z());
            glVertex3f(pmax.x(), pmax.y(), pmax.z());
            glVertex3f(pmin.x(), pmax.y(), pmax.z());

            glVertex3f(pmin.x(), pmin.y(), pmin.z());
            glVertex3f(pmin.x(), pmax.y(), pmin.z());
            glVertex3f(pmax.x(), pmax.y(), pmin.z());
            glVertex3f(pmax.x(), pmin.y(), pmin.z());

            glVertex3f(pmin.x(), pmin.y(), pmin.z());
            glVertex3f(pmin.x(), pmin.y(), pmax.z());
            glVertex3f(pmin.x(), pmax.y(), pmax.z());
            glVertex3f(pmin.x(), pmax.y(), pmin.z());

            glVertex3f(pmax.x(), pmin.y(), pmax.z());
            glVertex3f(pmax.x(), pmin.y(), pmin.z());
            glVertex3f(pmax.x(), pmax.y(), pmin.z());
            glVertex3f(pmax.x(), pmax.y(), pmax.z());

            glVertex3f(pmin.x(), pmax.y(), pmax.z());
            glVertex3f(pmax.x(), pmax.y(), pmax.z());
            glVertex3f(pmax.x(), pmax.y(), pmin.z());
            glVertex3f(pmin.x(), pmax.y(), pmin.z());

            glVertex3f(pmin.x(), pmin.y(), pmax.z());
            glVertex3f(pmin.x(), pmin.y(), pmin.z());
            glVertex3f(pmax.x(), pmin.y(), pmin.z());
            glVertex3f(pmax.x(), pmin.y(), pmax.z());

        glEnd();
    }
}
