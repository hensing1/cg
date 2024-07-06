#include "scene.hpp"

Scene02::Scene02() {
    campus.load("meshes/Campus.obj");
    sphere.load("meshes/highpolysphere.obj");
}

void Scene02::render(int frame, float time, Program& program, MovableCamera& camera) {
    camera.updateIfChanged();
    camera.setViewDirAlongSpline(time / 5);
    camera.setPosAlongSpline(time / 5);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    vec3 pos = vec3(0, 0, 0);
    mat4 worldToClip = camera.projectionMatrix * camera.viewMatrix;
    program.set("uColor", vec3(0.25f, 0.21f, 0.4f));
    this->drawMesh(0.5f, pos, program, campus, worldToClip);


    program.set("uColor", vec3(0.65f, 0.00f, 0.4f));
    for (int i = 0; i < view_path_points.size(); i++) {
        drawMesh(0.20f, view_path_points[i].pos, program, sphere, worldToClip);
    }


}

void Scene02::init(MovableCamera &camera) {
    path_points = {
        quintic_hermite_point{vec3(0.2f, 0.0f, PI/2-0.01), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(0.2f, 0.0f, PI/2-0.01), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(0.2f, PI, 0.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(0.2f, PI, 0.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(0.2f, PI+0.5, 0.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(0.2f, PI+0.5, 0.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
    };
    camera.setPath(QuinticHermite(&path_points));
    view_path_points = {
        quintic_hermite_point{vec3(0.0f, 20.0f, 0.0f), vec3(0.0f, -30.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(0.0f, 10.0f, 0.0f), vec3(0.0f, -10.0f, -3.0f), vec3(0.0f, 4.0f, 1.0f)},
        quintic_hermite_point{vec3(0.0f, 2.0f, -5.0f), vec3(0.0f, -1.6f, 0.0f), vec3(0.0f, 0.3f, 0.0f)},
        quintic_hermite_point{vec3(0.0f, 0.5f, -5.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(-0.8f, 0.5f, -4.0f), vec3(0.2f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(2.8f, 0.5f, 1.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
    };
    camera.setViewDirPath(QuinticHermite(&view_path_points));
}

Scene02::~Scene02() {}
