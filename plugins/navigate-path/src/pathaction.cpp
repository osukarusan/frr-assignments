#include "pathaction.h"
#include <iostream>

void PathActionMove::update(float, Scene *scene, Camera *cam)
{
    float srad = scene->boundingBox().radius();

    Vector speed = srad*this->movement/this->t_length;
    cam->pan(speed);
    cam->updateClippingPlanes(scene->boundingBox());

    this->t_elapsed += 1.0f;
}

void PathActionRotate::update(float, Scene *scene, Camera *cam)
{
    Vector speed = this->rotations/this->t_length;
    cam->rotateLocal(speed);
    cam->updateClippingPlanes(scene->boundingBox());

    this->t_elapsed += 1.0f;
}
