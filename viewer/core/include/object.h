#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "box.h"
#include "face.h"
#include "materiallib.h"

using namespace std;

// Object - permet representar un objecte 3D
class CORE_EXPORT Object
{
public:
    Object(std::string name);

    void readObj(const char* filename); // llegeix fitxer OBJ
    Box boundingBox() const;  // previament s'ha d'haver calculat
    
    const vector<Face>& faces() const;  // retorna una referencia al vector de cares de l'objecte
    const vector<Vertex>& vertices() const;
    vector<Vertex>& vertices();  // retorna una referencia al vector de vertexs de l'objecte
                                 // IMPORTANT: si modifiqueu algun vertex, cal que despr�s crideu computeNormals() i computeBoundingBox
    
    void computeNormals();     // re-calcula nomes les normals *per-cara*
    void computeVertexNormals(); // recalcula les normals fent la mitjana de les cares
    void computeBoundingBox(); // calcula la capsa

    void computeVertexNormalsPerFace();

    void setPosition(const Point& p); // p passa a ser la posicio del centre

    Object getCopy() const;
    Object toTriangles() const;        // converteix totes les cares a triangles

private:
    void make_face ( char **words, int nwords, int material );
    void clear();

    vector<Vertex>  pvertices;  // vector amb els vertexs de l'objecte
    vector<Face>    pfaces;     // vector amb les cares de l'objecte

    std::string pname;
    Box pboundingBox;   // caixa contenidora de l'objecte.

};

#endif

