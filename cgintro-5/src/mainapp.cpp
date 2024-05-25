#include "mainapp.hpp"

#include <imgui.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
using namespace glm;

#include <iostream>
#include <vector>
#include <cassert>

#include "framework/imguiutil.hpp"
#include "framework/app.hpp"
#include "framework/camera.hpp"
#include "framework/mesh.hpp"
#include "framework/gl/program.hpp"
#include "config.hpp"

MainApp::MainApp() : App(800, 600) {
    App::setTitle(Config::PROJECT_NAME);
    program.load("projection.vert", "lambert.frag");
    mesh.load("meshes/highpolysphere.obj");
    camera.sphericalPosition = vec3(5.0f, 0.0f, 1.5f);
}

void MainApp::init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
}

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

void MainApp::clickCallback(Button button, Action action, Modifier modifier) {
    if (button == Button::LEFT && action == Action::PRESS) {
        // Convert screen coordinates to viewport coordinates
        vec2 scale;
        glfwGetWindowContentScale(window, &scale.x, &scale.y);
        vec2 screenPos = mouse * scale / resolution * 2.0f - 1.0f;
        screenPos.y *= -1.0f;
        screenPos.x *= camera.aspectRatio;

        float t = 0.0f; // TODO perform raycasting with the XZ-plane

        // Check if the intersection is within the near and far planes
        if (t > camera.near && t < camera.far) {
            vec3 worldPos; // TODO calculate the world position of the clicked point
            std::cout << "Clicked: " << to_string(screenPos) << "\tAdded: " << to_string(worldPos) << std::endl;

            // Add the point to the spline
            if (modifier >= Modifier::CTRL || spline.empty()) spline.push_back({worldPos}); // Start a new curve
            else spline.back().push_back(worldPos); // Add a point to the current curve
        }
    }
}

vec3 deCasteljau(const std::vector<vec3>& points, float t) {
    // TODO: Implement De Casteljau's algorithm
    return vec3(0.0f);
}

vec3 evalSpline(const std::vector<std::vector<vec3>>& spline, float t, bool close) {
    assert(!spline.empty()); // Spline must contain at least one segment
    float tSpline = t * spline.size(); // Stretch t to the number of segments
    size_t i = floor(tSpline); // Segment index is the integer part of t
    float tSegment = fract(tSpline); // Position in segment is the fractional part of t
    std::vector<vec3> segment = spline[i]; // Copy current segment
    if (i + 1 < spline.size()) segment.push_back(spline[(i + 1)][0]); // Connect current segment to next segment
    else if (close) segment.push_back(spline[0][0]); // Connect last segment to start
    return deCasteljau(segment, tSegment); // Evaluate segment using De Casteljau's algorithm
}

void drawMesh(float size, const vec3& pos, Program& program, Mesh& mesh, const mat4& worldToClip) {
    mat4 localToWorld = scale(translate(mat4(1.0f), pos), vec3(size));
    program.set("uLocalToWorld", localToWorld);
    program.set("uLocalToClip", worldToClip * localToWorld);
    program.bind();
    mesh.draw();
}

void MainApp::render() {
    camera.updateIfChanged();
    mat4 worldToClip = camera.projectionMatrix * camera.viewMatrix;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw the control points
    srand(42); // Reset random seed for static colors
    for (auto segment : spline) {
        program.set("uColor", linearRand(vec3(0.0f), vec3(1.0f))); // Random color for each segment
        for (auto point : segment) {
            drawMesh(0.05f, point, program, mesh, worldToClip);
        }
    }

    // Draw points on the spline
    program.set("uColor", vec3(1.0f, 0.0f, 0.0f));
    if (!spline.empty()) {
        for (float d = 0.0f; d < 1.0f; d += 1.0f / numberOfBalls) {
            vec3 pos = evalSpline(spline, fract(time * speed + d), closed);
            drawMesh(0.1f, pos, program, mesh, worldToClip);
        }
    }
}

void MainApp::buildImGui() {
    ImGui::StatisticsWindow(App::delta, App::resolution);
    ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::SliderFloat("Speed", &speed, 0.0f, 1.0f);
    ImGui::SliderInt("Number of Balls", &numberOfBalls, 1, 100);
    ImGui::Text("Spline contains %zu Segments", spline.size());
    ImGui::Checkbox("Close Spline", &closed);
    if (ImGui::Button("Clear Spline")) spline.clear();
    ImGui::End();
}