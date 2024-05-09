#include "mainapp.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <imgui.h>

#include <vector>
#include <iostream>

#include "config.hpp"

using namespace glm;

const vec3 SUN_COLOR = vec3(1.0f, 0.6f, 0.0f);
const vec3 EARTH_COLOR = vec3(0.0f, 0.5f, 1.0f);
const vec3 MOON_COLOR = vec3(0.5f, 0.5f, 0.5f);

const float SUN_EARTH_DISTANCE = 5.0f; // in sun radii, in reality ca 149 000 000 km = ca 214 sun radii
const float EARTH_MOON_DISTANCE = 2.0f; // in earth radii, in reality ca 384 km = ca 60 earth radii
const float SUN_RADIUS = 1.0f; // In reality ca 696 340 km
const float SUN_EARTH_RATIO = 0.09f; // In reality ca 0.009
const float EARTH_MOON_RATIO = 0.27f; // This is actually accurate

const float TWO_PI = 2.0f * pi<float>();

MainApp::MainApp() : App(600, 600) {
    App::setTitle(Config::PROJECT_NAME);

    mesh.load("meshes/highpolysphere.obj");
    program.load("projection.vert", "color.frag");

    camera.sphericalPosition = vec3(10.0f, 0.0f, 0.0f); // position camera 10 units away
    camera.minDist = SUN_RADIUS + camera.near; // don't go inside the sun
}

// Handle input
void MainApp::keyCallback(Key key, Action action) { if (key == Key::ESC && action == Action::PRESS) App::close(); }
void MainApp::scrollCallback(float amount) { camera.zoom(amount); }
void MainApp::moveCallback(const vec2& movement, bool leftButton, bool rightButton, bool middleButton) { if (rightButton) camera.rotate(movement * 0.01f); }
void MainApp::resizeCallback(const vec2& resolution) { camera.resize(resolution.x / resolution.y); }

void MainApp::init() {
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
}

void MainApp::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float year = App::time / 5.0f; // 5 seconds for one year
    float month = year * 13.37f; // The Moon orbits the Earth ca 13.37 times a year

    camera.updateIfChanged();
    mat4 worldToClip = camera.projectionMatrix * camera.viewMatrix;

    mat4 sun = scale(vec3(SUN_RADIUS));

    mat4 earth = sun * rotate(year * TWO_PI, vec3(0.0f, 1.0f, 0.0f)) * translate(vec3(SUN_EARTH_DISTANCE, 0.0f, 0.0f)) * scale(vec3(SUN_EARTH_RATIO));

    // TODO: mat4 moon = ...

    // Get uniform locations only once
    static const GLuint uColor = program.uniform("uColor");
    static const GLuint uLocalToWorld = program.uniform("uLocalToWorld");
    static const GLuint uLocalToClip = program.uniform("uLocalToClip");
    static const GLuint uEnableLighting = program.uniform("uEnableLighting");

    program.bind();
    program.set(uColor, SUN_COLOR);
    program.set(uLocalToWorld, sun);
    program.set(uLocalToClip, worldToClip * sun);
    program.set(uEnableLighting, false);
    mesh.draw();
    program.set(uColor, EARTH_COLOR);
    program.set(uLocalToWorld, earth);
    program.set(uLocalToClip, worldToClip * earth);
    program.set(uEnableLighting, true);
    mesh.draw();
    // TODO: Draw moon
}