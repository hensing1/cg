#include "scene.hpp"
#include "framework/gl/texture.hpp"
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <iostream>


Scene03::Scene03(MovableCamera& camera) {
    program.load("Scene3.vert", "Scene3.frag");
    hoersaal.load("meshes/HS3.obj");
    holztexture.load(Texture::Format::SRGB8,"textures/Wood.png",0);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
 glm::vec3 lightDir = glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f));
    float metallness = 0.0f;
    bool useOrenNayar = false;
    float roughness = 1.0f;
program.set("uLightDir", lightDir);
program.set("uRoughness", roughness);
program.set("uMetallness", metallness);
program.set("uUseOrenNayar", useOrenNayar);
}

int Scene03::render(int frame, float time, MovableCamera& camera, bool DEBUG) {
    camera.updateIfChanged();
    glm::vec3 cameraPos = camera.cartesianPosition;
    // DO NOT ALLOW NEGATIVE VALUES FOR cameraPos otherwise part of the mesh turns Black
    if (cameraPos.x < 0)
        cameraPos.x = -cameraPos.x;
    if (cameraPos.y < 0)
        cameraPos.y = -cameraPos.y;
    if (cameraPos.z < 0)
        cameraPos.z = -cameraPos.z;
    program.set("uCameraPos", cameraPos);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    vec3 pos = vec3(0, 0, 0);
    mat4 worldToClip = camera.projectionMatrix * camera.viewMatrix;
    holztexture.bind(Texture::Type::TEX2D);
    this->drawMesh(1.0f, pos, program, hoersaal, worldToClip);
    return 0;
}

Scene03::~Scene03() {}
