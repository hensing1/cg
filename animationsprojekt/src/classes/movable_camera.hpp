
#pragma once

#include "framework/camera.hpp"
#include "hermite.hpp"

class MovableCamera : public Camera {
public:
    MovableCamera();
    MovableCamera(const vec3& sphericalPosition, const vec3& target, const vec3& up, float minDist, float maxDist, float fov);

    void setPath(QuinticHermite path);
    void setPosAlongSpline(float t);

private:
    QuinticHermite path;
};
