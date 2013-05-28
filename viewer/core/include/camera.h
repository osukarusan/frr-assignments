#ifndef CAMERA_H
#define CAMERA_H

#define _USE_MATH_DEFINES 1
#include "GL/glew.h"
#include "point.h"
#include "vector.h"
#include "box.h"
#include "frustum.h"

// Camera - permet representar una càmera virtual
class CORE_EXPORT Camera
{
public:
    void init(const Box&);
    void setModelview() const;
    void setProjection() const;
    Point getObs() const;
    void getAxes(Vector& camx, Vector& camy, Vector& camz);

    void getAngles(float& ax, float& ay, float& az) const;
    void setAngles(float ax, float ay, float az);

    float getAspectRatio() const;
    void setAspectRatio(float ar);

    void updateClippingPlanes(const Box&);
    
    void incrementDistance(float inc);
    void incrementAngleX(float inc);
    void incrementAngleY(float inc);
    
    void pan(const Vector& offset);
    void moveLocal(const Vector& offset);
    void rotateLocal(const Vector& angles);

    Frustum getFrustum() const;
    void getFrustumCorners(Vector corners[8]) const;
    void getFrustumCornersEyeSpace(Vector corners[8]) const;

    float getZnear() const;
    float getZfar() const;

private:
     // parametres de la camera
    Point pvrp;  // view reference point
    float pdist; // distancia obs-vrp
    float pangleX, pangleY, pangleZ;

    float pfovy; // fielf of view, vertical
    float paspectRatio; 
    float pzNear, pzFar; 
};

inline float Camera::getAspectRatio() const
{
    return paspectRatio;
}

inline float Camera::getZnear() const
{
    return pzNear;
}

inline float Camera::getZfar() const
{
    return pzFar;
}

#endif
