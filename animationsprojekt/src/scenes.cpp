
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

#include "config.hpp"
#include <iostream>


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
    program.set("uColor", vec3(1.0f, 1.0f, 0.0f));
    drawMesh(1.05f, vec3(2.0f, 4.0f, 0.0f), program, cube, worldToClip);
    //drawMesh(100.05f, vec3(-2.0f, 4.0f, 100.0f), program, cube, worldToClip);
    //drawMesh(100.05f, vec3(-2.0f, -100.0f, 0.0f), program, cube, worldToClip);
    //drawMesh(100.05f, vec3(100.0f, -3.0f, 0.0f), program, cube, worldToClip);

}

void MainApp::render_scene_02() {
    //  TODO: Szene 2
}



