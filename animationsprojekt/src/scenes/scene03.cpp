#include "scene.hpp"

Scene03::Scene03() {
    hoersaal.load("meshes/HS3.obj");
}

void Scene03::render(int frame, float time, Program& program, Camera& camera) {
    camera.updateIfChanged();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    vec3 pos = vec3(0, 0, 0);
    mat4 worldToClip = camera.projectionMatrix * camera.viewMatrix;
    this->drawMesh(1.0f, pos, program, hoersaal, worldToClip);
}

Scene03::~Scene03() {}
