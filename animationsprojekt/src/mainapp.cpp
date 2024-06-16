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

MainApp::MainApp() : App(800, 600) {
    App::setTitle(Config::PROJECT_NAME); // set title
    App::setVSync(true); // Limit framerate

    FRAME = 0;
    SCENE = 1;
    DEBUG_MODE = false;
    ANIMATION_PLAYING = true;

    App::imguiEnabled = false;
    camera.cartesianPosition = vec3(0.0f, 0.0f, 0.0f);
    
    //  NOTE: Nur zu Testzwecken -> später entfernen
    program.load("TMP_projection.vert", "TMP_lambert.frag");
    donut.load("meshes/donut.obj");
    cube.load("meshes/cube.obj");
    sphere.load("meshes/highpolysphere.obj");
}


void MainApp::init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
}



void MainApp::render() {
    
    /*  NOTE:  Jede Szene nutzt eine andere Render-Funktion.
     *         Es wird empfohlen, allgemeine Funktionalität in separate Funktionen auszulagern.
     *         (Wahrscheinlich sollten diese Funktionen auch in MainApp deklariert werden)
     */
    switch (SCENE) {
        case 1:
            render_scene_01();
        break;
        case 2:
            render_scene_02();
        break;
        default:
            render_scene_01();
        break;
    }

    // Framezahl erhöhen, wenn Animation abgespielt wird
    if (ANIMATION_PLAYING) FRAME++;
}


/*
 *  TODO: Keyboard-Clicks umsetzen
 */
void MainApp::keyCallback(Key key, Action action) {
    if (key == Key::ESC && action == Action::PRESS) App::close();
    else if (key == Key::COMMA && action == Action::PRESS) App::imguiEnabled = !App::imguiEnabled;  
    else if (key == Key::LEFT_SHIFT && action == Action::PRESS) std::cout << "HEHEHEHA" << std::endl;
}


/*
 *  TODO: Mouse-Clicks umsetzen
*/
void MainApp::clickCallback(Button button, Action action, Modifier modifier) {
    // Mouse
}

/*
 *  TODO: GUI umsetzen. GUI ist normalerweise ausgeschaltet, soll Debug-Funktionen zur Verfügung stellen.
 */
void MainApp::buildImGui() {
    ImGui::StatisticsWindow(App::delta, App::resolution);
    ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("Position:  (%f|%f|%f)", camera.cartesianPosition);
    ImGui::Text("Frame:  %zu", FRAME);
    ImGui::Text("Scene:  %zu", SCENE);
    ImGui::Checkbox("Debug Mode", &DEBUG_MODE);
    ImGui::Checkbox("Play Animation", &ANIMATION_PLAYING);
    ImGui::End();
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

