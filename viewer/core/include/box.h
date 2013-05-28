#ifndef BOX_H
#define BOX_H

#include "point.h"
#include "vector.h"


// Box - permet representar una capsa aliniada amb els eixos (ex. capsa englobant)
class CORE_EXPORT Box
{
public:
    Box(const Point& point=Point());
    Box(const Point& minimum, const Point& maximum);

    // expand - expandeix la capsa per que inclogui el punt donat
    void expand(const Point& p);

    // expand - expandeix la capsa per que inclogui la capsa donada
    void expand(const Box& p);

    // dibuixa la capsa en filferros)
    void render(bool wire = true);

    // retorna el centre de la capsa
    Point center() const;

    // retorna la meitat de la diagonal de la capsa
    float radius() const;

    void  getVertexs(Point vxs[8]) const;
    Point getPositiveVertex(const Vector& n) const;
    Point getNegativeVertex(const Vector& n) const;

    bool contains  (const Box& box) const;
    bool contains  (const Point& p) const;
    bool intersects(const Box& box) const;


private:
    // atributs
    Point pmin, pmax;	// extrems minim i maxim de la capsa

};

#endif

