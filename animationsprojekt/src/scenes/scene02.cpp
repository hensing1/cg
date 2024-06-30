#include "scene.hpp"

Scene02::Scene02() {
    campus.load("meshes/Campus.obj");
}

void Scene02::render(int frame, Program& program, Camera& camera) {
    camera.updateIfChanged();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    vec3 pos = vec3(0, 0, 0);
    mat4 worldToClip = camera.projectionMatrix * camera.viewMatrix;
    this->drawMesh(0.5f, pos, program, campus, worldToClip);
}

Scene02::~Scene02() {}
