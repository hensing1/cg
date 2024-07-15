#include "scene.hpp"
#include "framework/gl/texture.hpp"
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <iostream>


Scene03::Scene03(MovableCamera& camera) {
    program.load("Scene3.vert", "Scene3.frag");
    hoersaal.load("meshes/HS3.obj");
    bunny.load("meshes/bunny.obj");
    holztexture.load(Texture::Format::SRGB8,"textures/Wood.png",0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    blaetter.load(Texture::Format::SRGB8,"textures/Blaetter.jpg",0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
 glm::vec3 lightDir = glm::normalize(glm::vec3(1.0f, -1.0f, 0.0f));
    float metallness = 0.0f;
    bool useOrenNayar = true;
program.set("uLightDir", lightDir);
program.set("uMetallness", metallness);
program.set("uUseOrenNayar", useOrenNayar);
}

int Scene03::render(int frame, float time, MovableCamera& camera, bool DEBUG) {
    camera.updateIfChanged();
    glm::vec3 cameraPos = camera.cartesianPosition;
    program.set("uCameraPos", cameraPos);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    vec3 pos = vec3(0, 0, 0);
    mat4 worldToClip = camera.projectionMatrix * camera.viewMatrix;
    glActiveTexture(GL_TEXTURE0);
    holztexture.bind(Texture::Type::TEX2D);
    program.set("holztexture", 0);
    this->drawMesh(1.0f, pos, program, hoersaal, worldToClip);
    glActiveTexture(GL_TEXTURE1);
    blaetter.bind(Texture::Type::TEX2D);
    program.set("holztexture", 1);
    this->drawMesh(1.0f, pos, program, bunny, worldToClip);
    return 0;
}

Scene03::~Scene03() {}
