#include "vertex.h"

Vertex::Vertex()
{}

Vertex::Vertex(const Point& coord)
    : pcoord(coord)
{}

Vertex::Vertex(const Vertex &v) {
    pcoord = v.pcoord;
    pnormal = v.pnormal;
}

Point Vertex::coord() const
{
    return pcoord;
}

void Vertex::setCoord(const Point& coord)
{
    pcoord = coord;
}

Vector Vertex::normal() const {
    return pnormal;
}

void Vertex::setNormal(const Vector &n) {
    pnormal = n;
}


