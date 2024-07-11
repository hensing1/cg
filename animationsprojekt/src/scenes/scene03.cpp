#include "scene.hpp"

Scene03::Scene03(Program& program, MovableCamera& camera) {
    program.load("TMP_projection.vert", "TMP_lambert.frag");
    hoersaal.load("meshes/HS3.obj");
}

int Scene03::render(int frame, float time, Program& program, MovableCamera& camera, bool DEBUG) {
    camera.updateIfChanged();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    vec3 pos = vec3(0, 0, 0);
    mat4 worldToClip = camera.projectionMatrix * camera.viewMatrix;
    this->drawMesh(1.0f, pos, program, hoersaal, worldToClip);
    return 0;
}

Scene03::~Scene03() {}
