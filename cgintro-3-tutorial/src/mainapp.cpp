#include "mainapp.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

#include <vector>
#include <iostream>

#include "config.hpp"
#include "framework/imguiutil.hpp"

using namespace glm;

MainApp::MainApp() : App(800, 600) {
    App::setTitle(Config::PROJECT_NAME);
    
    mesh.load("meshes/suzanne.obj");
    program.load("projection.vert", "lambert.frag");
}

void MainApp::init() {
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
}

void MainApp::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    camera.updateIfChanged();

    mat4 model = rotate(mat4(1.0f), App::time, vec3(0.0f, 1.0f, 0.0f));
    mat4 view = camera.viewMatrix;
    mat4 projection = camera.projectionMatrix;

    mat4 localToClip = projection * view * model;
    mat4 localToWorld = model;

    program.set(program.uniform("uLocalToClip"), localToClip);
    program.set(program.uniform("uLocalToWorld"), localToWorld);
    program.set(program.uniform("uLightDir"), lightDir);
    program.set(program.uniform("uLightColor"), lightColor);
    program.set(program.uniform("uShowNormals"), showNormals);

    program.bind();
    mesh.draw();
}

void MainApp::keyCallback(Key key, Action action) {
    // Close the application when pressing ESC
    if (key == Key::ESC && action == Action::PRESS) App::close();
    // Toggle GUI with COMMA
    if (key == Key::COMMA && action == Action::PRESS) App::imguiEnabled = !App::imguiEnabled;
}

// Handle mouse input
void MainApp::scrollCallback(float amount) { camera.zoom(amount); }
void MainApp::moveCallback(const vec2& movement, bool leftButton, bool rightButton, bool middleButton) { if (rightButton) camera.rotate(movement * 0.01f); }
void MainApp::resizeCallback(const vec2& resolution) { camera.resize(resolution.x / resolution.y); }

void MainApp::buildImGui() {
    ImGui::StatisticsWindow(App::delta, App::resolution);
    ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::SphericalSlider("Light Direction", lightDir);
    ImGui::ColorEdit3("Light Color", value_ptr(lightColor), ImGuiColorEditFlags_Float);
    ImGui::Checkbox("Show Normals", &showNormals);
    if (ImGui::SliderAngle("Field Of View", &camera.fov, 0.0f, 180.0f)) camera.invalidate();
    if (ImGui::Button("Load Bunny")) mesh.load("meshes/bunny.obj");
    if (ImGui::Button("Load Suzanne")) mesh.load("meshes/suzanne.obj");
    if (ImGui::Button("Load Sphere")) mesh.load("meshes/highpolysphere.obj");
    ImGui::End();
}