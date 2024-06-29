
#include "mainapp.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
using namespace glm;

#include "framework/app.hpp"
#include "framework/camera.hpp"
#include "framework/gl/program.hpp"
#include "framework/mesh.hpp"

#include "classes/hermite.hpp"

#include "config.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <string>

//  NOTE: Nur zu Testzwecken -> später entfernen
void drawMesh(float size, const vec3 &pos, Program &program, Mesh &mesh,
              const mat4 &worldToClip) {
    mat4 localToWorld = scale(translate(mat4(1.0f), pos), vec3(size));
    program.set("uLocalToWorld", localToWorld);
    program.set("uLocalToClip", worldToClip * localToWorld);
    program.bind();
    mesh.draw();
    srand(42); // Reset random seed for static colors
}

void MainApp::render_test_scene() {
    
}

void MainApp::render_scene_01() {}

void MainApp::render_scene_02() {}

void MainApp::render_scene_03() {}
