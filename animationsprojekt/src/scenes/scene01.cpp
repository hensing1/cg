#include "scene.hpp"

Scene01::Scene01() {}

void Scene01::render(int frame, float time, Program& program, MovableCamera& camera) {
    camera.updateIfChanged();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

Scene01::~Scene01() {}
