#include "scene.hpp"

Scene03::Scene03(MovableCamera& camera, Program& program) {
    hoersaal.load("meshes/HS3.obj");
    hullin.load(Texture::Format::SRGB8, "textures/Hullin.png", 0);
    hullin.bind(Texture::Type::TEX2D);
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
