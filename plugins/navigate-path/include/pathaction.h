#ifndef PATHACTION_H
#define PATHACTION_H

#include "vector.h"
#include "camera.h"
#include "scene.h"


class PathAction
{
public:
    enum ActionType { MOVE, ROTATE };

    PathAction() { t_elapsed = 0.0f; }
    virtual ~ PathAction() {}

    float      t_length;
    float      t_elapsed;

    virtual ActionType getType() const = 0;
    virtual void update(float dt, Scene* scene, Camera* cam) = 0;

};


class PathActionMove : public PathAction
{
public:
    PathActionMove() {}
    virtual ~ PathActionMove() {}

    Vector movement;

    virtual ActionType getType() const { return MOVE; }
    virtual void update(float dt, Scene* scene, Camera* cam);
};


class PathActionRotate : public PathAction
{
public:
    PathActionRotate() {}
    virtual ~ PathActionRotate() {}

    Vector rotations;

    virtual ActionType getType() const { return ROTATE; }
    virtual void update(float dt, Scene* scene, Camera* cam);
};


#endif // PATHACTION_H
