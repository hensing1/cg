#include "scene.hpp"

// #include <glad/glad.h>
// #include <glm/glm.hpp>
// using namespace glm;

#include "framework/gl/program.hpp"
#include "framework/mesh.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
// #include <string>

Scene::Scene() {}

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

void Scene::init(MovableCamera& camera) {}


Scene::~Scene() {}
