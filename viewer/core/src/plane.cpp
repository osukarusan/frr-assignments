#include "plane.h"

Plane::Plane(void) {
	m_equation[0] = 0;
	m_equation[1] = 0;
	m_equation[2] = 0;
	m_equation[3] = 0;
}

Plane::~Plane(void) 
{
}

Plane::Plane(const Plane& p) {
	m_equation[0] = p.m_equation[0];
	m_equation[1] = p.m_equation[1];
	m_equation[2] = p.m_equation[2];
	m_equation[3] = p.m_equation[3];
}

Plane::Plane(float a, float b, float c, float d) {
	m_equation[0] = a;
	m_equation[1] = b;
	m_equation[2] = c;
	m_equation[3] = d;
}

Plane::Plane(const Point& p1, const Point& p2, const Point& p3) {
        Vector u(p2, p1);
	u.normalize();
        Vector v(p3, p1);
	v.normalize();

        Vector n = u ^ v;
        float  d = -(p1.x()*n.x() + p1.y()*n.y() + p1.z()*n.z());

	m_equation[0] = n.x();
	m_equation[1] = n.y();
	m_equation[2] = n.z();
	m_equation[3] = d;
}

Plane::Plane(const Vector& nor, const Point& p) {
        Vector n = nor.normalized();
        float    d = -(p.x()*n.x() + p.y()*n.y() + p.z()*n.z());

	m_equation[0] = n.x();
	m_equation[1] = n.y();
	m_equation[2] = n.z();
	m_equation[3] = d;
}
