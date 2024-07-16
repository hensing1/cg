#include "scene.hpp"
#include "framework/gl/texture.hpp"
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <iostream>


Scene03::Scene03(MovableCamera& camera) {
    program.load("Scene3.vert", "Scene3.frag");
    walls.load("meshes/walls.obj");
    boden.load("meshes/boden.obj");
    holz.load("meshes/holzObjekte.obj");
    holztexture.load(Texture::Format::SRGB8, "textures/Wood.png", 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    bodenTex.load(Texture::Format::SRGB8, "textures/Boden.jpg", 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    wallTex.load(Texture::Format::SRGB8, "textures/Boden.jpg", 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glm::vec3 lightDir = glm::normalize(glm::vec3(1.0f, -1.0f, -1.0f));
    float metallness = 0.0f;
    bool useOrenNayar = true;
    float roughness = 0.5f;
    program.set("uLightDir", lightDir);
    program.set("uRoughness",roughness);
    program.set("uUseOrenNayar", useOrenNayar);
    program.set("uMetallness", metallness);
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
    this->drawMesh(1.0f, pos, program, holz, worldToClip);
    glActiveTexture(GL_TEXTURE1);
    bodenTex.bind(Texture::Type::TEX2D);
    program.set("holztexture", 1);
    this->drawMesh(1.0f, pos, program, boden, worldToClip);
    glActiveTexture(GL_TEXTURE2);
    wallTex.bind(Texture::Type::TEX2D);
    program.set("holztexture", 2);
    this->drawMesh(1.0f, pos, program, walls, worldToClip);
    return 0;
}

Scene03::~Scene03() {}
