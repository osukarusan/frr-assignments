#ifndef VECTOR_H
#define VECTOR_H

#include <QVector3D>
#include <iostream>
#include "point.h"

//typedef QVector3D Vector;

class Vector : public QVector3D {

public:
    Vector();
    Vector(const QVector3D& v);
    Vector(const Vector& v);
    Vector(float x, float y, float z);
    Vector(const Point& p1, const Point& p2);
};

std::ostream& operator<< (std::ostream &os, const Vector &p);

inline float  operator*(const Vector& v1, const Vector& v2) {
    return QVector3D::dotProduct(v1, v2);
}

inline Vector operator^(const Vector& v1, const Vector& v2) {
    return QVector3D::crossProduct(v1, v2);
}


#endif


