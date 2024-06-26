
#include "mainapp.hpp"

#include <glad/glad.h>
#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include "framework/app.hpp"
#include "framework/camera.hpp"
#include "framework/gl/program.hpp"
#include "framework/mesh.hpp"

#include "classes/hermite.hpp"

#include "config.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <string>
#include <glm/gtx/string_cast.hpp>





//  NOTE: Nur zu Testzwecken -> später entfernen
void drawMesh(float size, const vec3& pos, Program& program, Mesh& mesh, const mat4& worldToClip) {
    mat4 localToWorld = scale(translate(mat4(1.0f), pos), vec3(size));
    program.set("uLocalToWorld", localToWorld);
    program.set("uLocalToClip", worldToClip * localToWorld);
    program.bind();
    mesh.draw();
    srand(42); // Reset random seed for static colors
}





void MainApp::render_scene_01() {
    //  TODO: Szene 1
    camera.updateIfChanged();
    mat4 worldToClip = camera.projectionMatrix * camera.viewMatrix;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
    // Draw Test-Meshes
    program.set("uColor", vec3(sin(float(FRAME)/14), 1.0f, cos(float(FRAME)/20)));
    drawMesh(0.25f, vec3(0.0f, 0.0f, 0.0f), program, cube, worldToClip);
    program.set("uColor", vec3(0.0f, 0.9f, 0.1f));
    drawMesh(0.85f, cube_path.evaluateSplineAllowLoop(float(FRAME) / 200), program, cube, worldToClip);
    program.set("uColor", vec3(0.0f, 0.3f, 0.0f));
    drawMesh(2.25f, sphere_path.evaluateSplineAllowLoop(float(FRAME) / 100), program, sphere, worldToClip);
    program.set("uColor", vec3(0.5f, 0.0f, 0.8f));
    drawMesh(5.05f, donut_path.evaluateSpline(float(FRAME) / 200), program, donut, worldToClip);
    program.set("uColor", vec3(0.8f, 0.0f, 0.8f));
    drawMesh(1.35f, smooth_sphere_path.evaluateSplineAllowLoop(float(FRAME) / 200), program, sphere, worldToClip);

}

void MainApp::render_scene_02() {
    //  TODO: Szene 2
}



