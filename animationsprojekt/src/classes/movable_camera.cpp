
#include "movable_camera.hpp"

#include <glm/gtc/constants.hpp>


MovableCamera::MovableCamera() {}

MovableCamera::MovableCamera(const vec3& sphericalPosition, const vec3& target, const vec3& up, float near, float far, float fov) {
    Camera(sphericalPosition, target, up, near, far, fov);
}

void MovableCamera::setPath(QuinticHermite new_path) {
    path = new_path;
}

void MovableCamera::setPosAlongSpline(float t) {
    sphericalPosition = path.evaluateSplineAllowLoop(t);
    
    while (sphericalPosition.y < 0.0f) { sphericalPosition.y = sphericalPosition.y + 2*pi<float>(); }
    while (sphericalPosition.z < 0.0f) { sphericalPosition.z = sphericalPosition.z + 2*pi<float>(); }
    while (sphericalPosition.y >= 2*pi<float>()) { sphericalPosition.y = sphericalPosition.y - 2*pi<float>(); }
    while (sphericalPosition.z > 2*pi<float>()) { sphericalPosition.z = sphericalPosition.z - 2*pi<float>(); }
    //sphericalPosition.z = mod(glm::pi<float>() + sphericalPosition.z, (float) 2*glm::pi<float>()) - glm::pi<float>(); 
    update();
}

void MovableCamera::setViewDirPath(QuinticHermite path) {
    viewPath = path;
}

void MovableCamera::setViewDirAlongSpline(float t) {
    target = viewPath.evaluateSplineAllowLoop(t);
}
