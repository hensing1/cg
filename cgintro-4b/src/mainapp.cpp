#include "mainapp.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
using namespace glm;
#include <imgui.h>

#include <iostream>
#include <vector>

#include "config.hpp"
#include "framework/imguiutil.hpp"
#include "framework/objparser.hpp"

MainApp::MainApp() : App(800, 600) {
    App::setTitle(Config::PROJECT_NAME);
    mesh.load(FULLSCREEN_VERTICES, FULLSCREEN_INDICES);
    program.load("raygen.vert", "raytrace.frag");
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

void loadObj(Program& program, const std::string& filename) {
    std::vector<Mesh::VertexPCN> vertices;
    std::vector<unsigned int> indices;
    ObjParser::parse(filename, vertices, indices);

    // Output mesh size
    GLuint triangleCount = indices.size() / 3;
    std::cout << "Vertices: " << vertices.size() << "\tIndices: " << indices.size() << "\tFaces: " << triangleCount << std::endl;

    // Pass the vertices to the shader as vec4 array
    // Each vertex is 2 vec4s arranged as:
    // (vec4(Px, Py, Pz, Cx), vec4(Cy, Nx, Ny, Nz)) with P = Position, C = Texture Coordinate, N = Normal
    glProgramUniform4fv(program.handle, program.uniform("uVertices"), vertices.size() * 2, value_ptr(vertices[0].position));

    // Pass the indices to the shader as uvec3 array, each uvec3 being a triangle
    glProgramUniform3uiv(program.handle, program.uniform("uIndices"), triangleCount, indices.data());

    // Pass triangle count to the shader
    program.set("uTriangleCount", triangleCount);
}

void MainApp::init() {
    program.set("uView", uView);
    program.set("uLightDir", uLightDir);
    program.set("uLightColor", uLightColor);
    program.set("uNear", uNear);
    program.set("uFar", uFar);
    loadObj(program, "meshes/lowpolysphere.obj");
    program.bind();
}

void MainApp::render() {
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
    if (ImGui::Combo("View", &uView, {"Render", "Normals", "Depth", "Barycentrics"})) program.set("uView", uView);
    if (ImGui::SphericalSlider("Light Dir", uLightDir)) program.set("uLightDir", uLightDir);
    if (ImGui::ColorEdit3("Light Color", value_ptr(uLightColor), ImGuiColorEditFlags_Float)) program.set("uLightColor", uLightColor);
    if (ImGui::SliderFloat("Near Plane", &uNear, 0.0, 10.0, "%.1f")) program.set("uNear", uNear);
    if (ImGui::SliderFloat("Far Plane", &uFar, 0.0, 100.0, "%.1f", ImGuiSliderFlags_Logarithmic)) program.set("uFar", uFar);
    if (ImGui::Button("Load Plane")) loadObj(program, "meshes/plane.obj");
    if (ImGui::Button("Load Cube")) loadObj(program, "meshes/cube.obj");
    if (ImGui::Button("Load Sphere")) loadObj(program, "meshes/lowpolysphere.obj");
    ImGui::End();
}