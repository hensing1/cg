#include "mainapp.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;
#include <imgui.h>

#include "config.hpp"
#include "framework/imguiutil.hpp"

MainApp::MainApp() : App(800, 600) {
    App::setTitle(Config::PROJECT_NAME);
    mesh.load(FULLSCREEN_VERTICES, FULLSCREEN_INDICES);
    program.load("raygen.vert", "raymarch.frag");
    camera.sphericalPosition = vec3(6.0f, 0.8f, 0.4f);
}

// Handle input
void MainApp::keyCallback(Key key, Action action) {
    if (key == Key::ESC && action == Action::PRESS) App::close();
    else if (key == Key::COMMA && action == Action::PRESS) App::imguiEnabled = !App::imguiEnabled;
}

void MainApp::scrollCallback(float amount) {
    camera.zoom(amount);
}

void MainApp::moveCallback(const vec2& movement, bool leftButton, bool rightButton, bool middleButton) {
    if (rightButton | middleButton) camera.rotate(movement * 0.01f);
}

void MainApp::resizeCallback(const vec2& resolution) {
    camera.resize(resolution.x / resolution.y);
}

void MainApp::init() {
    program.set("uLightDir", uLightDir);
    program.set("uLightColor", uLightColor);
    program.set("uNear", uNear);
    program.set("uFar", uFar);
    program.set("uSteps", uSteps);
    program.set("uEpsilon", uEpsilon);
    program.set("uNormalEps", uNormalEps);
    program.bind();
}

void MainApp::render() {
    program.set(program.uniform("uTime"), App::time);

    if (camera.updateIfChanged()) {
        program.set("uCameraMatrix", camera.cameraMatrix);
        program.set("uAspectRatio", camera.aspectRatio);
        program.set("uFocalLength", camera.focalLength);
        program.set("uCameraPosition", camera.cartesianPosition);
    }

    mesh.draw();
}

void MainApp::buildImGui() {
    ImGui::StatisticsWindow(App::delta, App::resolution);

    ImGui::Begin("Settings", nullptr, ImGuiChildFlags_AlwaysAutoResize);
    if (ImGui::Combo("View", &uView, {"Render", "Normals", "Depth", "Steps"})) program.set("uView", uView);
    if (ImGui::SphericalSlider("Light Dir", uLightDir)) program.set("uLightDir", uLightDir);
    if (ImGui::ColorEdit3("Light Color", value_ptr(uLightColor), ImGuiColorEditFlags_Float)) program.set("uLightColor", uLightColor);
    if (ImGui::SliderFloat("Near Plane", &uNear, 0.0, 10.0, "%.1f")) program.set("uNear", uNear);
    if (ImGui::SliderFloat("Far Plane", &uFar, 0.0, 100.0, "%.1f", ImGuiSliderFlags_Logarithmic)) program.set("uFar", uFar);
    if (ImGui::SliderFloat("Max Steps", &uSteps, 0.0, 200.0, "%.f")) program.set("uSteps", uSteps);
    if (ImGui::SliderFloat("Epsilon", &uEpsilon, 0.0, 1.0, "%.6f", ImGuiSliderFlags_Logarithmic)) program.set("uEpsilon", uEpsilon);
    if (ImGui::SliderFloat("Normal Epsilon", &uNormalEps, 0.0, 1.0, "%.6f", ImGuiSliderFlags_Logarithmic)) program.set("uNormalEps", uNormalEps);
    ImGui::End();
}