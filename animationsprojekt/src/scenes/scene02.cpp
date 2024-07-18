#include "mesh.hpp"
#include "scene.hpp"
#include "framework/gl/texture.hpp"
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <iostream>

Scene02::Scene02() {

    cloudProgram.load("sc2_clouds.vert", "sc2_clouds.frag");
    cloudCanvas.load(FULLSCREEN_VERTICES, FULLSCREEN_INDICES);

    program.load("Scene2.vert", "Scene2.frag");

    campusBoden.load("meshes/sc2_boden.obj");
    sphere.load("meshes/highpolysphere.obj");
    buildings.load("meshes/sc2_gebaeude.obj");
    kronen.load("meshes/Baumkronen.obj");
    stamm.load("meshes/Baumstamm.obj");

    bodenTex.load(Texture::Format::SRGB8,"textures/karte.png",0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    buildingsTex.load(Texture::Format::SRGB8,"textures/Campusgebaeude.png",0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    stammTex.load(Texture::Format::SRGB8,"textures/Baumrinde.jpg",0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    kronenTex.load(Texture::Format::SRGB8,"textures/Blaetter.jpg",0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    

    camera_path_points = {
        // Der Fall nach unten
        quintic_hermite_point{vec3(0.02f, 0.0f, glm::pi<float>()/2), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(0.02f, 0.0f, glm::pi<float>()/2-0.1f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        // Positionierung am Gang
        quintic_hermite_point{vec3(0.02f, glm::pi<float>(), 0.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
          //quintic_hermite_point{vec3(0.02f, glm::pi<float>(), 0.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(0.02f, glm::pi<float>()+0.5, 0.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        // Flug durch den Gang
        quintic_hermite_point{vec3(0.02f, glm::pi<float>()+0.5, 0.0f), vec3(0.0f, glm::pi<float>()/2, 0.0f), vec3(0.0f, -glm::pi<float>()/16, 0.0f)},
        // Weg zum Hörsaal
        quintic_hermite_point{vec3(0.02f, 0.0f, 0.0f), vec3(0.0f, -glm::pi<float>()/8, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
          //quintic_hermite_point{vec3(0.02f, -glm::pi<float>()/8, 0.0f), vec3(0.0f, -glm::pi<float>()/8, 0.0f), vec3(0.0f, glm::pi<float>()/16, 0.0f)},
        quintic_hermite_point{vec3(0.02f, -glm::pi<float>()+glm::pi<float>()/4, 0.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(0.02f, -glm::pi<float>()+glm::pi<float>()/4, 0.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
    };
    view_path_points = {
        // Der Fall nach unten
        quintic_hermite_point{vec3(0.0f, 20.0f, 0.0f), vec3(0.0f, -10.0f, 0.0f), vec3(0.0f, -2.0f, -1.0f)},
        quintic_hermite_point{vec3(0.0f, 10.0f, 0.0f), vec3(0.0f, -8.0f, -2.5f), vec3(0.0f, -5.5f, 0.3f)},
        // Positionierung am Gang
        quintic_hermite_point{vec3(0.0f, 2.0f, -5.0f), vec3(0.0f, -2.5f, 0.0f), vec3(0.0f, 1.3f, -0.2f)},
          //quintic_hermite_point{vec3(0.0f, 0.5f, -5.0f), vec3(-0.2f, -0.3f, -1.0f), vec3(-1.0f, 0.0f, 2.2f)},
        quintic_hermite_point{vec3(-0.8f, 0.2f, -4.0f), vec3(3.2f, 0.0f, 2.5f), vec3(0.6f, 0.0f, 2.5f)},
        // Flug durch den Gang
        quintic_hermite_point{vec3(2.8f, 0.2f, 1.0f), vec3(6.0f, 0.0f, 6.2f), vec3(-4.0f, 0.025f, -4.0f)},
        // Weg zum Hörsaal
        quintic_hermite_point{vec3(-1.5f, 0.2f, 4.1f), vec3(-3.2f, 0.05f, -2.0f), vec3(2.2f, 0.05f, -1.4f)},
          //quintic_hermite_point{vec3(-2.3f, 0.3f, 1.3f), vec3(0.8f, 0.1f, 0.0f), vec3(0.5f, -0.05f, 0.0f)},
        quintic_hermite_point{vec3(0.62f, 0.4f, 0.2f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(4.62f, 0.2f, 3.2f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
    };
}

void Scene02::render_debug_objects(Program& program, mat4 worldToClip, vec3 playerPosition) {
    program.set("uColor", vec3(0.65f, 0.00f, 0.4f));
    for (int i = 0; i < view_path_points.size(); i++) {
        drawMesh(0.20f, view_path_points[i].pos, program, sphere, worldToClip);
    }
    
    program.set("uColor", vec3(0.6f, 0.6f, 0.6f));
    this->drawMesh(0.05f, vec3(0.0f, 3.0f, 0.0f), program, sphere, worldToClip);
    program.set("uColor", vec3(1.0f, 0.0f, 0.0f));
    this->drawMesh(0.05f, vec3(0.12f, 3.0f, 0.0f), program, sphere, worldToClip);
    program.set("uColor", vec3(0.0f, 1.0f, 0.0f));
    this->drawMesh(0.05f, vec3(0.0f, 3.12f, 0.0f), program, sphere, worldToClip);
    program.set("uColor", vec3(0.0f, 0.0f, 1.0f));
    this->drawMesh(0.05f, vec3(0.0f, 3.0f, 0.12f), program, sphere, worldToClip);
    program.set("uColor", vec3(0.0f, 1.0f, 1.0f));
    this->drawMesh(0.02f, playerPosition, program, sphere, worldToClip);
}

int Scene02::render(int frame, float time, MovableCamera& camera, bool DEBUG) {
    if (!DEBUG) {
        camera.setViewDirAlongSpline(time / 3);
        camera.setPosAlongSpline(time / 3);
    }
    camera.updateIfChanged();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    cloudProgram.bind();
    cloudProgram.set("uTime", time);
    cloudProgram.set("uCameraPosition", camera.cartesianPosition);
    cloudProgram.set("uCameraMatrix", camera.cameraMatrix);
    cloudProgram.set("uAspectRatio", camera.aspectRatio);
    cloudProgram.set("uFocalLength", camera.focalLength);
    cloudProgram.set("uLightDir", lightDir);

    cloudCanvas.draw();

    glClear(GL_DEPTH_BUFFER_BIT); // Wolken immer im Hintergrund

    program.bind();
    mat4 worldToClip = camera.projectionMatrix * camera.viewMatrix;
    mat4 localToWorld = scale(mat4(1.0f), vec3(0.5f));
    program.set("uLocalToWorld", localToWorld);
    program.set("uLocalToClip", worldToClip * localToWorld);
    program.set("uLightDir", lightDir);

    bodenTex.bind(Texture::Type::TEX2D, 0);
    program.set("uTexture", 0);
    campusBoden.draw();

    buildingsTex.bind(Texture::Type::TEX2D, 1);
    program.set("uTexture", 1);
    buildings.draw();
    
    glActiveTexture(GL_TEXTURE2);
    kronenTex.bind(Texture::Type::TEX2D, 2);
    program.set("uTexture", 2);
    kronen.draw();

    glActiveTexture(GL_TEXTURE3);
    stammTex.bind(Texture::Type::TEX2D, 3);
    program.set("uTexture", 3);
    stamm.draw();

    if (DEBUG) render_debug_objects(program, worldToClip, camera.getViewDirAlongSpline(time / 4));

    if (time >= 18.5f) return 3;
    return 0;
}


Scene02::~Scene02() {}
