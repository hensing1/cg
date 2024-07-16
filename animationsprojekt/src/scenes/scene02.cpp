#include "scene.hpp"
#include "framework/gl/texture.hpp"
#include <glm/glm.hpp>
#include <glad/glad.h>

Scene02::Scene02(MovableCamera& camera) {
    program.load("Scene2.vert", "Scene2.frag");
    campus.load("meshes/Campus.obj");
    sphere.load("meshes/highpolysphere.obj");
    boden.load(Texture::Format::SRGB8,"textures/Wood.png",0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    buildings.load(Texture::Format::SRGB8,"textures/Blaetter.jpg",0);
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
        quintic_hermite_point{vec3(0.02f, -glm::pi<float>()+glm::pi<float>()/3, 0.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(0.02f, -glm::pi<float>()+glm::pi<float>()/3, 0.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
    };
    camera.setPath(QuinticHermite(&camera_path_points));
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
    camera.setViewDirPath(QuinticHermite(&view_path_points));
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
        camera.setViewDirAlongSpline(time / 4);
        camera.setPosAlongSpline(time / 4);
    }
    camera.updateIfChanged();

    mat4 worldToClip = camera.projectionMatrix * camera.viewMatrix;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    vec3 campus_pos(0.0f);
    glActiveTexture(GL_TEXTURE0);
    buildings.bind(Texture::Type::TEX2D);
    program.set("uTexture", 0);
    this->drawMesh(0.5f, campus_pos, program, campus, worldToClip);
    if (DEBUG) render_debug_objects(program, worldToClip, camera.getViewDirAlongSpline(time / 4));

    if (time >= 24.7f) return 3;
    return 0;
}

Scene02::~Scene02() {}
