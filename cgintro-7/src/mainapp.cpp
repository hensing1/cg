#include "mainapp.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

#include "framework/app.hpp"
#include "framework/camera.hpp"
#include "framework/imguiutil.hpp"
#include "framework/mesh.hpp"
#include "framework/gl/program.hpp"
#include "framework/gl/texture.hpp"

using namespace glm;

MainApp::MainApp() : App(800, 600) {
    mesh.load("meshes/bunny.obj");

    meshShader.load("projection.vert", "pbr.frag");
    meshShader.set("uAlbedo", uAlbedo);
    meshShader.set("uRoughness", uRoughness);
    meshShader.set("uMetallness", uMetallness);
    meshShader.set("uUseOrenNayar", uUseOrenNayar);
    meshShader.set("uDebugView", uDebugView);
    meshShader.set("uDistribution", uDistribution);
    meshShader.set("uLightDir", uLightDirection);
}

void MainApp::init() {
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
}

void MainApp::render() {
    if (cam.updateIfChanged()) {
        meshShader.set("uWorldToClip", cam.projectionMatrix * cam.viewMatrix);
        meshShader.set("uCameraPos", cam.cartesianPosition);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //mat4 localToWorld = rotate(mat4(1.0f), time, vec3(0.0f, 1.0f, 0.0f));
    //meshShader.set("uLocalToWorld", localToWorld);
    meshShader.bind();
    mesh.draw();
}

/* Catch window events by overriding the callback functions */
void MainApp::keyCallback(Key key, Action action) {
    if (key == Key::ESC && action == Action::PRESS) close();
    if (key == Key::COMMA && action == Action::PRESS) App::imguiEnabled = !App::imguiEnabled;
}
void MainApp::scrollCallback(float amount) {
    cam.zoom(amount);
}
void MainApp::moveCallback(const vec2& movement, bool leftButton, bool rightButton, bool middleButton) {
    if (leftButton | rightButton | middleButton) cam.rotate(movement * 0.01f);
}
void MainApp::resizeCallback(const vec2& resolution) {
    cam.resize(resolution.x / resolution.y);
}

const std::vector<std::string> debugViews = {"Off", "D", "F", "G", "specular", "diffuse", "orenNayar", "N", "V", "H", "NdotL", "NdotV", "NdotH", "HdotV", "worldPos"};
const std::vector<std::string> meshes = {"meshes/bunny.obj", "meshes/cube.obj", "meshes/cylinder.obj", "meshes/donut.obj", "meshes/highpolysphere.obj", "meshes/plane.obj", "meshes/suzanne.obj"};

/* Build GUI elements here using the functions from imgui.h and util.cpp */
void MainApp::buildImGui() {
    ImGui::StatisticsWindow(delta, resolution);

    ImGui::Begin("Config", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    if (ImGui::Combo("Debug View", &uDebugView, debugViews)) meshShader.set("uDebugView", uDebugView);
    if (ImGui::Combo("Mesh", &meshID, meshes)) mesh.load(meshes[meshID]);
    if (ImGui::Combo("Distribution", &uDistribution, {"Beckmann", "Trowbridge-Reitz (GGX)"})) meshShader.set("uDistribution", uDistribution);
    if (ImGui::SliderFloat("Roughness", &uRoughness, 0.0f, 1.0f)) meshShader.set("uRoughness", uRoughness);
    if (ImGui::SliderFloat("Metallness", &uMetallness, 0.0f, 1.0f)) meshShader.set("uMetallness", uMetallness);
    if (ImGui::Checkbox("Use Oren-Nayar", &uUseOrenNayar)) meshShader.set("uUseOrenNayar", uUseOrenNayar);
    if (ImGui::ColorEdit3("Albedo", value_ptr(uAlbedo), ImGuiColorEditFlags_Float)) meshShader.set("uAlbedo", uAlbedo);
    if (ImGui::SphericalSlider("Light Direction", uLightDirection)) meshShader.set("uLightDir", uLightDirection);
    ImGui::End();
}