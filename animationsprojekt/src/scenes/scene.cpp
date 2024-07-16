#include "scene.hpp"

Scene::Scene() {}

void Scene::setCameraPath(MovableCamera& camera) {
    camera.setPath(QuinticHermite(&camera_path_points));
    camera.setViewDirPath(QuinticHermite(&view_path_points));
}

//  NOTE: Nur zu Testzwecken -> später entfernen
void Scene::drawMesh(float size, const vec3& pos, Program& program, Mesh& mesh,
                     const mat4& worldToClip) {
    mat4 localToWorld = scale(translate(mat4(1.0f), pos), vec3(size));
    program.set("uLocalToWorld", localToWorld);
    program.set("uLocalToClip", worldToClip * localToWorld);
    program.bind();
    mesh.draw();
    srand(42); // Reset random seed for static colors
}

//  NOTE: Nur zu Testzwecken -> später entfernen
void Scene::drawMesh(float size, const vec3& pos, Program& program, Mesh& mesh,
                     const mat4& worldToClip, const mat4& transformation) {
    mat4 localToWorld = scale(translate(mat4(1.0f), pos), vec3(size));
    program.set("uLocalToWorld", localToWorld);
    program.set("uLocalToClip", worldToClip * localToWorld * mat4(transformation) );
    program.bind();
    mesh.draw();
    srand(42); // Reset random seed for static colors
}

Scene::~Scene() {}
