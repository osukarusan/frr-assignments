#include "scene.h"

Scene::Scene() : pselectedObject(0)  // inicialment l'objecte seleccionat és el 0
{}

Box Scene::boundingBox() const
{
    return pboundingBox;
}

const vector<Object>& Scene::objects() const
{
    return pobjects;
}
    
void Scene::addObject(Object &o)
{
    pobjects.push_back(o);
    computeBoundingBox();
}

void Scene::computeBoundingBox()
{
    if (pobjects.size())
    {
        pboundingBox = pobjects[0].boundingBox();
        for (unsigned int i=1; i<pobjects.size(); ++i)
            pboundingBox.expand(pobjects[i].boundingBox()); 
    }
}

int Scene::selectedObject() const 
{
    if (pobjects.size()) return pselectedObject;
    else return -1; 
}

void Scene::setSelectedObject(int index)
{
    pselectedObject = index;
}

void Scene::replicateObject(int index, int repx, int repy, int repz) {
    Box bbox = pobjects[index].boundingBox();
    float d = 2.0f*bbox.radius();
    Point c = bbox.center();
    for (int i = 0; i < repx; i ++) {
        for (int j = 0; j < repy; j++) {
            for (int k = 0; k < repz; k++) {
                if (i == j && j == k && k == 0) continue;
                Object o2 = pobjects[index].getCopy();
                Point t(i*d, j*d, k*d);
                o2.setPosition(c + t);
                o2.computeBoundingBox();
                pobjects.push_back(o2);
            }
        }
    }
    computeBoundingBox();
}
