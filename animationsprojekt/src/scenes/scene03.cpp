#include "operations.hpp"
#include "scene.hpp"
#include "classes/hermite.hpp"
#include "framework/gl/texture.hpp"
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <iostream>
#include <vector>
#include <glm/gtx/string_cast.hpp>


Scene03::Scene03(MovableCamera& camera) {
    program.load("Scene3.vert", "Scene3.frag");
    walls.load("meshes/walls.obj");
    boden.load("meshes/BodenHS.obj");
    holz.load("meshes/holzObjekte.obj");
    sphere.load("meshes/highpolysphere.obj");
    hullin.load("meshes/plane.obj");
    
    holztexture.load(Texture::Format::SRGB8, "textures/Wood.png", 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    bodenTex.load(Texture::Format::SRGB8, "textures/Boden.jpg", 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    wallTex.load(Texture::Format::SRGB8, "textures/Wand.jpg", 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    hullinTex.load(Texture::Format::SRGB8, "textures/Hullin.png", 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glm::vec3 lightDir = glm::normalize(glm::vec3(1.0f, -1.0f, 0.0f));
    float metallness = 0.0f;
    bool useOrenNayar = true;
    float roughness = 0.5f;
    program.set("uLightDir", lightDir);
    program.set("uRoughness",roughness);
    program.set("uUseOrenNayar", useOrenNayar);
    program.set("uMetallness", metallness);
    camera_path_points = {
        // Der Fall nach unten
        quintic_hermite_point{vec3(0.02f, 0.0f, 0.0f), vec3(0.0f, 0.0f, PI/16), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(0.02f, 0.0f, PI/16), vec3(0.0f, 0.0f, PI/16), vec3(0.0f, 0.0f, PI/2)},
        quintic_hermite_point{vec3(0.02f, 0.0f, PI/14), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -PI/8)},
        quintic_hermite_point{vec3(0.02f, PI/32, -PI/32), vec3(0.0f, PI/8, -PI/16), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(0.02f, PI/32, -PI/16), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(0.02f, PI/32, -PI/16), vec3(0.0f, PI/16, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(0.02f, PI/16, -PI/16), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(0.02f, PI/16, -PI/16), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(0.02f, PI/16, -PI/16), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(0.02f, PI/16, -PI/16), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(0.02f, PI/16, -PI/16), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(0.02f, PI/16, -PI/16), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
    };
    camera.setPath(QuinticHermite(&camera_path_points));
    view_path_points = {
        // Flug an den Platz
        quintic_hermite_point{vec3(1.416f, 4.277f, 9.275f), vec3(-1.424f, -0.5f, -3.283f), vec3(-0.2f, -0.3f, -0.4f)},
        //quintic_hermite_point{vec3(-0.208f, -0.281f, -3.208f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(-0.281f, 2.949f, 3.121f), vec3(0.0f, -2.224f, -6.2f), vec3(0.0f, -0.5f, 3.0f)},
        quintic_hermite_point{vec3(-0.281f, 0.255f, -0.010f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(-0.281f, 0.255f, -0.010f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(-0.281f, 0.255f, -0.010f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(-0.281f, 0.255f, -0.010f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(-0.281f, 0.255f, -0.010f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(-0.281f, 0.255f, -0.010f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(-0.281f, 0.255f, -0.010f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(-0.281f, 0.255f, -0.010f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(-0.281f, 0.255f, -0.010f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(-0.281f, 0.255f, -0.010f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(-0.281f, 0.255f, -0.010f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},

    };
    camera.setViewDirPath(QuinticHermite(&view_path_points));



    this -> hullinPathPoints = {
        hermite_point{vec3( 6.7341f, -1.0f, -12.1000f), vec3(0.0f,  0.0f, 0.0f)},
        hermite_point{vec3( 6.7341f, -1.0f, -12.1000f), vec3(0.0f,  0.0f, 0.0f)},
        hermite_point{vec3( 6.7341f, -1.0f, -12.1000f), vec3(0.0f,  0.0f, 0.0f)},
        hermite_point{vec3( 6.7341f, -1.0f, -12.1000f), vec3(0.0f,  0.0f, 0.0f)},
        hermite_point{vec3( 6.7341f, -1.0f, -12.1000f), vec3(0.0f,  0.0f, 0.0f)},
        hermite_point{vec3( 6.7341f, -1.0f, -12.1000f), vec3(0.0f,  0.0f, 0.0f)},
        hermite_point{vec3( 6.7341f, -1.0f, -12.1000f), vec3(0.0f,  0.0f, 0.0f)},
        hermite_point{vec3( 6.7341f, -1.0f, -12.1000f), vec3(0.0f,  0.0f, 0.0f)},
        hermite_point{vec3( 6.7341f, -1.0f, -12.1000f), vec3(0.0f,  0.0f, 0.0f)},
        hermite_point{vec3( 6.7341f, -1.0f, -12.1000f), vec3(0.0f,  0.0f, 0.0f)},
        hermite_point{vec3( 6.7341f, -1.0f, -12.1000f), vec3(0.0f,  0.0f, 0.0f)},
        hermite_point{vec3( 6.7341f, -1.0f, -12.1000f), vec3(0.0f,  0.0f, 0.0f)},
        // Hullin schaut kurz rein
        hermite_point{vec3( 5.8298f, -1.0f, -10.8305f), vec3(0.0f,  0.0f, 0.0f)},
        hermite_point{vec3( 3.8298f, -1.0f, -10.8305f), vec3(0.0f,  0.0f, 0.0f)},
        hermite_point{vec3( 3.8298f, -1.0f, -10.8305f), vec3(0.0f,  0.0f, 0.0f)},
        hermite_point{vec3( 5.8298f, -1.0f, -10.8305f), vec3(0.0f,  0.0f, 0.0f)},
        hermite_point{vec3( 6.7341f, -1.0f, -12.1000f), vec3(0.0f,  0.0f, 0.0f)},
        // Hullin läuft rein
        hermite_point{vec3( 3.1000f, -1.0f, -11.8246f), vec3(0.0f,  0.8f, 0.0f)},
        hermite_point{vec3( 2.0000f, -1.0f, -09.0236f), vec3(0.0f, -0.8f, 0.0f)},
        hermite_point{vec3( 0.9000f, -1.0f, -09.0236f), vec3(0.0f,  0.8f, 0.0f)},
        hermite_point{vec3(-0.2000f, -1.0f, -09.0236f), vec3(0.0f, -0.8f, 0.0f)},
        hermite_point{vec3(-1.3000f, -1.0f, -09.0236f), vec3(0.0f,  0.8f, 0.0f)},
        hermite_point{vec3(-2.4000f, -1.0f, -09.0236f), vec3(0.0f, -0.8f, 0.0f)},
        hermite_point{vec3(-3.5000f, -0.8f, -09.0236f), vec3(0.0f,  0.8f, 0.0f)},
        // Hullin nimmt seinen rechtmäßigen Platz ein
        hermite_point{vec3(-4.9081f, -0.8f, -09.0236f), vec3(0.0f,  0.0f, 0.0f)},
        hermite_point{vec3(-4.9081f, -0.2f, -08.9779f), vec3(0.0f,  0.0f, 0.0f)},
        hermite_point{vec3(-4.9081f, -0.2f, -08.9779f), vec3(0.0f,  0.0f, 0.0f)},
        hermite_point{vec3(-4.9081f, -0.2f, -08.9779f), vec3(0.0f,  0.0f, 0.0f)},
        hermite_point{vec3(-4.9081f, -0.2f, -08.9779f), vec3(0.0f,  0.0f, 0.0f)},
    };
    this -> hullinPath.set_path(hullinPathPoints);

    this -> hullinRotationPathPoints = {
        hermite_point{vec3(PI/2, 0.0f, 0.0f), vec3(0.0f, 0.0f, -PI/8)},
        hermite_point{vec3(PI/2, 0.0f, 0.0f), vec3(0.0f, 0.0f, -PI/8)},
        hermite_point{vec3(PI/2, 0.0f, 0.0f), vec3(0.0f, 0.0f, -PI/8)},
        hermite_point{vec3(PI/2, 0.0f, 0.0f), vec3(0.0f, 0.0f, -PI/8)},
        hermite_point{vec3(PI/2, 0.0f, 0.0f), vec3(0.0f, 0.0f, -PI/8)},
        hermite_point{vec3(PI/2, 0.0f, 0.0f), vec3(0.0f, 0.0f, -PI/8)},
        hermite_point{vec3(PI/2, 0.0f, 0.0f), vec3(0.0f, 0.0f, -PI/8)},
        hermite_point{vec3(PI/2, 0.0f, 0.0f), vec3(0.0f, 0.0f, -PI/8)},
        hermite_point{vec3(PI/2, 0.0f, 0.0f), vec3(0.0f, 0.0f, -PI/8)},
        hermite_point{vec3(PI/2, 0.0f, 0.0f), vec3(0.0f, 0.0f, -PI/8)},
        hermite_point{vec3(PI/2, 0.0f, 0.0f), vec3(0.0f, 0.0f, -PI/8)},
        hermite_point{vec3(PI/2, 0.0f, 0.0f), vec3(0.0f, 0.0f, -PI/8)},
        // Hullin schaut kurz rein
        hermite_point{vec3(PI/2, 0.0f, 0.0f), vec3(0.0f, 0.0f,  0.0f)},
        hermite_point{vec3(PI/2, 0.0f, PI/8), vec3(0.0f, 0.0f,  0.0f)},
        hermite_point{vec3(PI/2, 0.0f, PI/8), vec3(0.0f, 0.0f,  0.0f)},
        hermite_point{vec3(PI/2, 0.0f, 0.0f), vec3(0.0f, 0.0f,  0.0f)},
        hermite_point{vec3(PI/2, PI/16, 0.0f), vec3(0.0f, 0.0f,  0.0f)},
        // Hullin läuft rein
        hermite_point{vec3(PI/2, PI/4, 0.0f), vec3(0.0f, 0.0f,  PI/8)},
        hermite_point{vec3(PI/2, PI/8, 0.0f), vec3(0.0f, 0.0f, -PI/8)},
        hermite_point{vec3(PI/2, 0.0f, 0.0f), vec3(0.0f, 0.0f,  PI/8)},
        hermite_point{vec3(PI/2, 0.0f, 0.0f), vec3(0.0f, 0.0f, -PI/8)},
        hermite_point{vec3(PI/2, 0.0f, 0.0f), vec3(0.0f, 0.0f,  PI/8)},
        hermite_point{vec3(PI/2, 0.0f, 0.0f), vec3(0.0f, 0.0f, -PI/8)},
        hermite_point{vec3(PI/2, 0.0f, 0.0f), vec3(0.0f, 0.0f,  PI/8)},
        // Hullin nimmt seinen rechtmäßigen Platz ein
        hermite_point{vec3(PI/2, 0.0f, 0.0f), vec3(0.0f, 0.0f,  0.0f)},
        hermite_point{vec3(PI/2, 0.0f, 0.0f), vec3(0.0f, 0.0f,  0.0f)},
        hermite_point{vec3(PI/2, 0.0f, 0.0f), vec3(0.0f, 0.0f,  0.0f)},
        hermite_point{vec3(PI/2, 0.0f, 0.0f), vec3(0.0f, 0.0f,  0.0f)},
        hermite_point{vec3(PI/2, 0.0f, 0.0f), vec3(0.0f, 0.0f,  0.0f)},
    };
    this -> hullinRotationPath.set_path(hullinRotationPathPoints);
    std::cout << to_string(hullinRotationPath.get_path()[1].pos) << std::endl;
}


int Scene03::render(int frame, float time, MovableCamera& camera, bool DEBUG) {
    if (!DEBUG) {
        camera.setViewDirAlongSpline(time / 2);
        camera.setPosAlongSpline(time / 2);
    }
    camera.updateIfChanged();
    glm::vec3 cameraPos = camera.cartesianPosition;
    program.set("uCameraPos", cameraPos);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    vec3 pos = vec3(0.0f, -3.0f, 0.0f);
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
    glActiveTexture(GL_TEXTURE3);
    hullinTex.bind(Texture::Type::TEX2D);
    program.set("holztexture", 3);
    hullinPos = hullinPath.evaluateSplineAllowLoop(time*2);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    mat4 hullinMatrix = mat4(Operations::get_rotation_matrix(hullinRotationPath.evaluateSplineAllowLoop(time*2)));
    this->drawMesh(2.5f, hullinPos, program, hullin, worldToClip, hullinMatrix);
    glDisable(GL_BLEND);

    if (DEBUG) render_debug_objects(program, worldToClip, camera.getViewDirAlongSpline(time / 2), camera.target);
    return 0;
}

void Scene03::render_debug_objects(Program& program, mat4 worldToClip, vec3 playerPosition, vec3 target) {
    program.set("uColor", vec3(0.65f, 0.00f, 0.4f));
    for (int i = 0; i < view_path_points.size(); i++) {
        drawMesh(0.10f, view_path_points[i].pos, program, sphere, worldToClip);
    }
    
    program.set("uColor", vec3(0.6f, 0.6f, 0.6f));
    this->drawMesh(0.1f, vec3(0.0f, 0.0f, 0.0f), program, sphere, worldToClip);
    program.set("uColor", vec3(1.0f, 0.0f, 0.0f));
    this->drawMesh(0.1f, vec3(0.12f, 0.0f, 0.0f), program, sphere, worldToClip);
    program.set("uColor", vec3(0.0f, 1.0f, 0.0f));
    this->drawMesh(0.1f, vec3(0.0f, 0.12f, 0.0f), program, sphere, worldToClip);
    program.set("uColor", vec3(0.0f, 0.0f, 1.0f));
    this->drawMesh(0.1f, vec3(0.0f, 0.0f, 0.12f), program, sphere, worldToClip);
    program.set("uColor", vec3(0.0f, 1.0f, 1.0f));
    this->drawMesh(0.03f, playerPosition, program, sphere, worldToClip);
    program.set("uColor", vec3(1.0f, 1.0f, 0.0f));
    this->drawMesh(0.01f, target, program, sphere, worldToClip);
}


Scene03::~Scene03() {}
