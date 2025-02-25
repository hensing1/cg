
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
    vec3 getViewDirAlongSpline(float t);

    void move_target(const vec3 &movement);

private:
    QuinticHermite path;
    QuinticHermite viewPath;
};
