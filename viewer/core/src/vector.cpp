#include "vector.h"


Vector::Vector() : QVector3D() {
}

Vector::Vector(const QVector3D& v) : QVector3D(v) {
}

Vector::Vector(const Vector& v) : QVector3D(v) {
}

Vector::Vector(float x, float y, float z) : QVector3D(x, y, z) {
}

Vector::Vector(const Point &p1, const Point &p2) : QVector3D(p2 - p1) {
}
