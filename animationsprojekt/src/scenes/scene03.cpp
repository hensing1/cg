#include "scene.hpp"
#include "framework/gl/texture.hpp"


Scene03::Scene03(MovableCamera& camera) {
    program.load("Scene3.vert", "Scene3.frag");
    hoersaal.load("meshes/HS3.obj");
    holztexture.load(Texture::Format::SRGB8,"textures/Wood.png",0);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

int Scene03::render(int frame, float time, MovableCamera& camera, bool DEBUG) {
    camera.updateIfChanged();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    vec3 pos = vec3(0, 0, 0);
    mat4 worldToClip = camera.projectionMatrix * camera.viewMatrix;
    holztexture.bind(Texture::Type::TEX2D);
    this->drawMesh(1.0f, pos, program, hoersaal, worldToClip);
    return 0;
}

Scene03::~Scene03() {}
