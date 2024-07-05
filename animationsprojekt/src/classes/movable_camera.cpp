
#include "movable_camera.hpp"


#define PI 3.141592653589792

MovableCamera::MovableCamera() {}

MovableCamera::MovableCamera(const vec3& sphericalPosition, const vec3& target, const vec3& up, float near, float far, float fov) {
    Camera(sphericalPosition, target, up, near, far, fov);
}

void MovableCamera::setPath(QuinticHermite new_path) {
    path = new_path;
}

void MovableCamera::setPosAlongSpline(float t) {
    sphericalPosition = path.evaluateSplineAllowLoop(t);
    sphericalPosition.y = mod(PI + sphericalPosition.y, (float) 2*PI) - PI; 
    sphericalPosition.z = mod(PI + sphericalPosition.z, (float) 2*PI) - PI; 
    update();
}

void MovableCamera::setViewDirPath(Hermite path) {
    viewPath = path;
}

void MovableCamera::setViewDirAlongSpline(float t) {
    target = viewPath.evaluateSplineAllowLoop(t);
}
