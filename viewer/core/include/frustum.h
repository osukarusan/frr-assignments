#ifndef _FRUSTUM_H_
#define _FRUSTUM_H_

#include "core_global.h"
#include "point.h"
#include "vector.h"
#include "plane.h"
#include "box.h"

class CORE_EXPORT Frustum
{

public:

	enum FrustumPlane {FP_LEFT, FP_RIGHT, FP_TOP, FP_BOTTOM, FP_NEAR, FP_FAR};

	// Constructores
	Frustum(void);
	~Frustum(void);

	// Modificadores
        void setParams(const Point& obs, const Point& vrp, const Vector& vup,
                       float ar, float fov, float znear, float zfar);
	
	// Consultores
        Plane getPlane(FrustumPlane plane)                           const;
        bool  contains(const Point& p)                               const;
        bool  contains(const Point& c, float r)                      const;
        bool  contains(const Box& b)			             const;
        bool  intersects(const Point& c, float r, bool solid = true) const;
        bool  intersects(const Box& b, bool solid = true)            const;

private:

	Plane m_planes[6];

};

inline Plane Frustum::getPlane(FrustumPlane plane) const {
	return m_planes[plane];
}

#endif
