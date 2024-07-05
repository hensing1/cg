#include "scene.hpp"

Scene02::Scene02() {
    campus.load("meshes/Campus.obj");
}

void Scene02::render(int frame, float time, Program& program, MovableCamera& camera) {
    camera.updateIfChanged();
    camera.setViewDirAlongSpline(time / 5);
    camera.setPosAlongSpline(time / 5);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    vec3 pos = vec3(0, 0, 0);
    mat4 worldToClip = camera.projectionMatrix * camera.viewMatrix;
    this->drawMesh(0.5f, pos, program, campus, worldToClip);
}

void Scene02::init(MovableCamera &camera) {
    std::vector path_points = {
        quintic_hermite_point{vec3(0.2f, 0.0f, PI/2-0.01), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f)},
        quintic_hermite_point{vec3(0.2f, 0.0f, PI/2-0.01), vec3(0.0f, 1.0f, -1.0f), vec3(0.0f)},
        quintic_hermite_point{vec3(0.2f, PI, 0.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f)},
        quintic_hermite_point{vec3(0.2f, PI, 0.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f)},
    };
    camera.setPath(QuinticHermite(&path_points));
    std::vector view_path_points = {
        hermite_point{vec3(0.0f, 20.0f, 0.0f), vec3(0.0f, -30.0f, 0.0f)},
        hermite_point{vec3(0.0f, 10.0f, 0.0f), vec3(0.0f, -15.0f, -3.0f)},
        hermite_point{vec3(0.0f, 2.0f, -6.0f), vec3(0.0f, -1.6f, 0.0f)},
        hermite_point{vec3(0.0f, 0.5f, -6.0f), vec3(0.0f, 0.0f, 0.0f)},
    };
    camera.setViewDirPath(Hermite(&view_path_points));
}

Scene02::~Scene02() {}
