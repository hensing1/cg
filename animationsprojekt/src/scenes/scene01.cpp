#include "scene.hpp"

Scene01::Scene01(MovableCamera& camera) {}

int Scene01::render(int frame, float time, Program& program, MovableCamera& camera, bool DEBUG) {
    camera.updateIfChanged();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    return 0;
}

Scene01::~Scene01() {}
