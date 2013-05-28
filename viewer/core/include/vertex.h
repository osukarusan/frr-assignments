#ifndef VERTEX_H
#define VERTEX_H

#include "point.h"
#include "vector.h"

// Vertex - representa un vertex d'un objecte 3D
class CORE_EXPORT Vertex
{
public:
    Vertex();
    Vertex(const Point& coordenades);
    Vertex(const Vertex& v);

    Point coord() const;
    Vector normal() const;              

    void setCoord(const Point& coord);
    void setNormal(const Vector& n);

private:
    Point pcoord;		// coordenades del vertex
    Vector pnormal;
};

#endif
