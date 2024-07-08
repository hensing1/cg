
#pragma once

#include "framework/camera.hpp"
#include "hermite.hpp"
#include <glm/glm.hpp>

class MovableCamera : public Camera {
public:
    MovableCamera();
    MovableCamera(const vec3& sphericalPosition, const vec3& target, const vec3& up, float minDist, float maxDist, float fov);

    void setPath(QuinticHermite path);
    void setPosAlongSpline(float t);
    void setViewDirPath(QuinticHermite path);
    void setViewDirAlongSpline(float t);

private:
    QuinticHermite path;
    QuinticHermite viewPath;
};
