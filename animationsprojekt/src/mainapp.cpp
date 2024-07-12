#include "mainapp.hpp"
#include "scenes/scene.hpp"

#include <glad/glad.h>
#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
using namespace glm;

#include "framework/app.hpp"
#include "framework/gl/program.hpp"
#include "framework/imguiutil.hpp"

#include "classes/movable_camera.hpp"

#include "config.hpp"
#include <iostream>

MainApp::MainApp() : App(800, 600) {
    
    App::setTitle(Config::PROJECT_NAME); // set title
    App::setVSync(true); // Limit framerate

    camera = MovableCamera();

    oceanColor = vec3(0, 46, 212) / 255.f;
    landColor = vec3(10, 71, 0) / 255.f;
    mountainColor = vec3(1,1,1);
    landThreshold = 0.15f;
    applyHeightmap = true;

    FRAME = 0;
    SCENE = prev_scene = 1;
    DEBUG_MODE = false;
    ANIMATION_PLAYING = true;

    App::imguiEnabled = true;
    camera.cartesianPosition = vec3(0.0f, 0.0f, 0.0f);
    
    //  NOTE: Nur zu Testzwecken -> später entfernen
    // program.load("TMP_projection.vert", "TMP_lambert.frag");

    scene_start_time = 0.0f;
    current_time = 0.0f;
    switchScene();
}


void MainApp::init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
}



void MainApp::render() {

    
    // Framezahl erhöhen, wenn Animation abgespielt wird
    if (ANIMATION_PLAYING) FRAME++;
    else scene_start_time += time - scene_start_time;

    if (SCENE != prev_scene) { // event listener für arme
        switchScene();
        reset_time_in_scene();
    }
    
    if (SCENE == 1) {
        current_scene->program.set("oceanColor", oceanColor);
        current_scene->program.set("landColor", landColor);
        current_scene->program.set("mountainColor", mountainColor);
        current_scene->program.set("landThreshold", landThreshold);
        current_scene->program.set("applyHeightmap", applyHeightmap);
    }

    int scene_return = current_scene->render(FRAME, time - scene_start_time, camera, DEBUG_MODE);

    prev_scene = SCENE;
    prev_time = time;

    if (scene_return != 0) SCENE = scene_return; // Rückgabe = 0:  Keine Änderung;   sonst neue Scene
}

void MainApp::switchScene() {
    FRAME = 0;
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    switch (SCENE) {
    case 0:
        current_scene = std::make_unique<TestScene>(camera);
        break;
    case 1:
        current_scene = std::make_unique<Scene01>(camera);
        break;
    case 2:
        current_scene = std::make_unique<Scene02>(camera);
        break;
    case 3:
        current_scene = std::make_unique<Scene03>(camera);
        break;
    default:
        current_scene = std::make_unique<TestScene>(camera);
    }
    // current_scene -> init(camera);
}

/*
 *  NOTE: Keyboard-Clicks  ->  Nutzung in Readme erklärt
 */
void MainApp::keyCallback(Key key, Action action) {
    if (key == Key::ESC && action == Action::PRESS) App::close();
    else if (key == Key::COMMA && action == Action::PRESS) App::imguiEnabled = !App::imguiEnabled;  
    //else if (key == Key::LEFT_SHIFT && action == Action::PRESS) std::cout << "HEHEHEHA" << std::endl;
    else if (key == Key::BACKSPACE && action == Action::PRESS) reset_time_in_scene();

    else if (key == Key::W          && (action == Action::PRESS || action == Action::REPEAT)) wasdCallback(vec3( 0.0f, 0.0f, 1.0f));
    else if (key == Key::S          && (action == Action::PRESS || action == Action::REPEAT)) wasdCallback(vec3( 0.0f, 0.0f, -1.0f));
    else if (key == Key::D          && (action == Action::PRESS || action == Action::REPEAT)) wasdCallback(vec3( 1.0f, 0.0f,  0.0f));
    else if (key == Key::A          && (action == Action::PRESS || action == Action::REPEAT)) wasdCallback(vec3(-1.0f, 0.0f,  0.0f));
    else if (key == Key::LEFT_SHIFT && (action == Action::PRESS || action == Action::REPEAT)) wasdCallback(vec3( 0.0f,-1.0f,  0.0f));
    else if (key == Key::SPACE      && (action == Action::PRESS || action == Action::REPEAT)) wasdCallback(vec3( 0.0f, 1.0f,  0.0f));
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

    ImGui::Text("Frame:  %u    \t\t|   Scene:  %u", FRAME, SCENE);
    ImGui::Text("Time:   %f \t| Scene-Start:   %f \t Global-Time:  %f", current_time, scene_start_time, time);
    if (ImGui::Button("Reset Time")) reset_time_in_scene();
    ImGui::Text("---------------------------------------------------");

    ImGui::Checkbox("Debug Mode", &DEBUG_MODE);
    ImGui::Checkbox("Play Animation", &ANIMATION_PLAYING);
    ImGui::RadioButton("Test scene", &SCENE, 0);
    ImGui::RadioButton("Scene 1", &SCENE, 1);
    ImGui::RadioButton("Scene 2", &SCENE, 2);
    ImGui::RadioButton("Scene 3", &SCENE, 3);
    if (ImGui::CollapsingHeader("Options for scene 1")) {
        ImGui::Checkbox("Apply heightmap", &applyHeightmap);
        ImGui::SliderFloat("Land threshold", &landThreshold, 0.01, 1);
        ImGui::ColorPicker3("Ocean color", (float*) &oceanColor);
        ImGui::ColorPicker3("Land color", (float*) &landColor);
        ImGui::ColorPicker3("Mountain color", (float*) &mountainColor);
    }
    ImGui::End();
}

void MainApp::reset_time_in_scene() {
    scene_start_time = time - floor(time);
    current_time = scene_start_time;
}



void MainApp::scrollCallback(float amount) {
    camera.zoom(amount);
}

void MainApp::moveCallback(const vec2& movement, bool leftButton, bool rightButton, bool middleButton) {
    //std::cout << std::to_string(movement.x) << "|" << std::to_string(movement.y) << std::endl;
    if (rightButton | middleButton) camera.rotate(movement * 0.01f);
}

void MainApp::resizeCallback(const vec2& resolution) {
    camera.resize(resolution.x / resolution.y);
}

void MainApp::wasdCallback(const vec3 &movement) {
    camera.move_target(movement);
}

