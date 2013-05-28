#include "frustum.h"
#include <cmath>

Frustum::Frustum(void)
{
}

Frustum::~Frustum(void)
{
}

void Frustum::setParams(const Point& obs, const Point& vrp, const Vector& vup,
                                float ratio, float fov, float znear, float zfar)
{
	// Calculem el sistema de referencia de la camera
        Vector zCam = Vector(obs) - Vector(vrp);
        zCam.normalize();
        Vector xCam = vup  ^ zCam;
        Vector yCam = zCam ^ xCam;

	// Calculem l'alcada i l'amplada dels plans de retallat
        float hNear = 2 * tan(DEG2RAD*fov/2) * znear;
	float wNear = hNear * ratio;

	// Calculem el centre del pla near i els extrems
        Point nc  = obs - zCam * znear;
        Point fc  = obs - zCam * zfar;
        Point ncr =  nc + xCam * (wNear / 2);
        Point ncl =  nc - xCam * (wNear / 2);
        Point nct =  nc + yCam * (hNear / 2);
        Point ncb =  nc - yCam * (hNear / 2);

	// Calculem les normals dels plans laterals
        Vector aux;
        Vector vobs = Vector(obs);
        aux = Vector(ncr) - vobs;	aux.normalize();
        Vector normalRight  = yCam ^ aux;
        aux = Vector(ncl) - vobs;	aux.normalize();
        Vector normalLeft   = aux ^ yCam;
        aux = Vector(nct) - vobs;	aux.normalize();
        Vector normalTop    = aux ^ xCam;
        aux = Vector(ncb) - vobs;	aux.normalize();
        Vector normalBottom = xCam ^ aux;

	// Calculem els plans
	m_planes[FP_LEFT]   = Plane(normalLeft,   ncl);
	m_planes[FP_RIGHT]  = Plane(normalRight,  ncr);
	m_planes[FP_TOP]    = Plane(normalTop,    nct);
	m_planes[FP_BOTTOM] = Plane(normalBottom, ncb);
	m_planes[FP_NEAR]   = Plane(-zCam, nc);
	m_planes[FP_FAR]    = Plane( zCam, fc);

}


bool Frustum::contains(const Point& p) const {
	// si el punt es al semiespai negatiu d'algun pla, esta a fora
	for (int i = 0; i < 6; i++) {
		if (m_planes[i].getSignedDistance(p) < 0) return false;
	}
	return true;
}

bool Frustum::contains(const Point& c, float r) const {
	// si alguna distancia centre-pla es menor al radi, talla o es fora
	for (int i = 0; i < 6; i++) {
		if (m_planes[i].getSignedDistance(c) < r) return false;
	}
	return true;
}

bool Frustum::contains(const Box& b) const {
	// per cada pla
	for (int i = 0; i < 6; i++) {
                Point pvert = b.getPositiveVertex(m_planes[i].getNormal());
                Point nvert = b.getNegativeVertex(m_planes[i].getNormal());
		// si el vertex positiu es al semiespai negatiu, la capsa es fora
		if (m_planes[i].getSignedDistance(pvert) < 0)      return false;
		// sino, si el vertex negatiu es al semiespai negatiu, la capsa talla
		else if (m_planes[i].getSignedDistance(nvert) < 0) return false;
	}
	return true;
}

bool Frustum::intersects(const Point& c, float r, bool solid) const {
	bool interseccio = false;
	for (int i = 0; i < 6; i++) {
                float dist = m_planes[i].getSignedDistance(c);
		// si la distancia es major al radi pel semiespai negatiu, es fora
		if      (dist < -r)	return false;
		// altrament, si no es major al radi al semiespai positiu, talla
		else if (dist <  r) interseccio = true;
	}
	// si hem arribat fins aqui, sabem que esta a dins o talla algun pla
	// si nomes volem interseccio explicita (solid = fals) retornarem si hi
	// ha hagut alguna. Sino, ens es igual si es dins o si talla
	return solid || interseccio;
}

bool Frustum::intersects(const Box& b, bool solid) const {
	bool interseccio = false;
	// per cada pla
	for (int i = 0; i < 6; i++) {
                Point pvert = b.getPositiveVertex(m_planes[i].getNormal());
                Point nvert = b.getNegativeVertex(m_planes[i].getNormal());
		// si el vertex positiu es al semiespai negatiu, la capsa es fora
		if (m_planes[i].getSignedDistance(pvert) < 0)      return false;
		// sino, si el vertex negatiu es al semiespai negatiu, la capsa talla
		else if (m_planes[i].getSignedDistance(nvert) < 0) interseccio = true;
	}
	// si hem arribat fins aqui, sabem que esta a dins o talla algun pla
	// si nomes volem interseccio explicita (solid = fals) retornarem si hi
	// ha hagut alguna. Sino, ens es igual si es dins o si talla
	return solid || interseccio;
}

