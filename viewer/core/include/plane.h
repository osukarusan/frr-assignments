#ifndef _PLANE_
#define _PLANE_

#include "core_global.h"
#include "point.h"
#include "vector.h"


class CORE_EXPORT Plane
{

public:
	
	// Constructors
	Plane(void);
	Plane(const Plane& p);
        Plane(float a, float b, float c, float d);
        Plane(const Point& p1, const Point& p2, const Point& p3);
        Plane(const Vector& n, const Point& p);
	~Plane(void);

	// Consultores
        void    getEquation(Vector& n, float& d)    const;
        Vector  getNormal()                         const;
        float	GetCoefficientD()                   const;
        float	getDistance(const Point& p)         const;
        float	getSignedDistance(const Point& p)   const;
        bool	isPointOnPlane(const Point& p)      const;
        bool	isVectorOnPlane(const Vector& v)    const;


private:

        float	m_equation[4];

};

inline void Plane::getEquation(Vector& n, float& d) const {
        n = Vector(m_equation[0], m_equation[1], m_equation[2]);
	d = m_equation[3];
}

inline Vector Plane::getNormal() const {
        return Vector(m_equation[0], m_equation[1], m_equation[2]);
}

inline float Plane::GetCoefficientD() const {
	return m_equation[3];
}


inline float Plane::getDistance(const Point& p) const {
	return ABS(getSignedDistance(p));
}

inline float Plane::getSignedDistance(const Point& p) const {
	return   p.x()*m_equation[0] + p.y()*m_equation[1] 
	       + p.z()*m_equation[2] + m_equation[3];
}

inline bool Plane::isPointOnPlane(const Point& p) const {
	return getDistance(p) < TOLERANCE;
}

inline bool Plane::isVectorOnPlane(const Vector& v) const {
        Vector n(m_equation[0], m_equation[1], m_equation[2]);
        return ABS(v*n) < TOLERANCE;
}

#endif
