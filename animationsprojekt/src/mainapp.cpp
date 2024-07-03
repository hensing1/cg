#include "mainapp.hpp"
#include "scenes/scene.hpp"

#include <glad/glad.h>
#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
using namespace glm;

#include "framework/app.hpp"
#include "framework/camera.hpp"
#include "framework/gl/program.hpp"
#include "framework/mesh.hpp"
#include "framework/imguiutil.hpp"

#include "classes/hermite.hpp"
#include "classes/movable_camera.hpp"

#include "config.hpp"
#include <iostream>

MainApp::MainApp() : App(800, 600) {
    App::setTitle(Config::PROJECT_NAME); // set title
    App::setVSync(true); // Limit framerate

    camera = MovableCamera();

    FRAME = 0;
    SCENE = prev_scene = 0;
    DEBUG_MODE = false;
    ANIMATION_PLAYING = true;

    App::imguiEnabled = true;
    camera.cartesianPosition = vec3(0.0f, 0.0f, 0.0f);
    
    //  NOTE: Nur zu Testzwecken -> später entfernen
    program.load("TMP_projection.vert", "TMP_lambert.frag");

    current_scene = std::make_unique<TestScene>();
    current_scene -> init(camera);
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

    if (SCENE != prev_scene) { // event listener für arme
        switchScene();
    }
    current_scene->render(FRAME, time, program, camera);
    prev_scene = SCENE;

    // Framezahl erhöhen, wenn Animation abgespielt wird
    if (ANIMATION_PLAYING) FRAME++;
}

void MainApp::switchScene() {
    FRAME = 0;
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    switch (SCENE) {
    case 0:
        current_scene = std::make_unique<TestScene>();
        break;
    case 1:
        current_scene = std::make_unique<Scene01>();
        break;
    case 2:
        current_scene = std::make_unique<Scene02>();
        break;
    case 3:
        current_scene = std::make_unique<Scene03>();
        break;
    default:
        current_scene = std::make_unique<TestScene>();
    current_scene -> init(camera);
    }
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
    ImGui::Text("Position:  (%f|%f|%f)", camera.cartesianPosition[0], camera.cartesianPosition[1], camera.cartesianPosition[2]);
    ImGui::Text("Frame:  %u", FRAME);
    ImGui::Text("Scene:  %u", SCENE);
    ImGui::Checkbox("Debug Mode", &DEBUG_MODE);
    ImGui::Checkbox("Play Animation", &ANIMATION_PLAYING);
    ImGui::RadioButton("Test scene", &SCENE, 0);
    ImGui::RadioButton("Scene 1", &SCENE, 1);
    ImGui::RadioButton("Scene 2", &SCENE, 2);
    ImGui::RadioButton("Scene 3", &SCENE, 3);
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

