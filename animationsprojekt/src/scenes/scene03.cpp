#include "scene.hpp"

Scene03::Scene03() {}

void Scene03::render(int frame, Program& program, Camera& camera) {
    camera.updateIfChanged();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

Scene03::~Scene03() {}
