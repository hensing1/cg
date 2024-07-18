#include "mainapp.hpp"
#include "scenes/scene.hpp"

#include <cwchar>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <memory>
using namespace glm;

#include "framework/app.hpp"
#include "framework/gl/program.hpp"
#include "framework/imguiutil.hpp"

#include "classes/movable_camera.hpp"

#include "config.hpp"

MainApp::MainApp() : App(800, 600) {

    App::setTitle(Config::PROJECT_NAME); // set title
    App::setVSync(true);                 // Limit framerate

    camera = MovableCamera();

    // Szenen initialisieren, um shader zu kompilieren etc.
    // scene_01 = std::make_shared<Scene01>();
    // scene_02 = std::make_shared<Scene02>();
    // scene_03 = std::make_shared<Scene03>();

    sc1_oceanColor = vec3(0, 8, 69) / 255.f;
    sc1_landColor = vec3(10, 71, 0) / 255.f;
    sc1_mountainColor = vec3(1, 1, 1);
    sc1_landThreshold = 0.15f;
    sc1_lightDir = normalize(vec3(0.6, 0.2, 0.8));
    sc1_alpha = -0.883f;
    sc1_beta = 6.181f;
    sc1_gamma = 0;
    sc1_atmosColor = vec3(0, 23, 83) / 255.f;
    sc3_roughness = 0.5f;

    FRAME = 0;
    SCENE = prev_scene = 1;
    DEBUG_MODE = false;
    ANIMATION_PLAYING = true;

    App::imguiEnabled = true;
    camera.cartesianPosition = vec3(0.0f, 0.0f, 0.0f);

    //  NOTE: Nur zu Testzwecken -> später entfernen
    // program.load("TMP_projection.vert", "TMP_lambert.frag");

    scene_start_time = 0.0f;
    current_time = 2.4f;
    switchScene();
}

void MainApp::init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
}

void MainApp::render() {

    // Framezahl erhöhen, wenn Animation abgespielt wird
    if (ANIMATION_PLAYING) {
        FRAME++;
        current_time =
            current_time + (time - prev_time); // Current Time läuft nur, wenn Animation spielt
    }

    if (SCENE != prev_scene) { // event listener für arme
        switchScene();
        reset_time_in_scene();
    }

    if (SCENE == 1) {
        Scene01* scene = (Scene01*)current_scene.get();
        scene->program.set("uLightDir", sc1_lightDir);
        scene->program.set("oceanColor", sc1_oceanColor);
        scene->program.set("landColor", sc1_landColor);
        scene->program.set("mountainColor", sc1_mountainColor);
        scene->program.set("landThreshold", sc1_landThreshold);

        scene->cloudProgram.set("uLightDir", sc1_lightDir);
        scene->cloudProgram.set("uAlpha", sc1_alpha);
        scene->cloudProgram.set("uBeta", sc1_beta);
        scene->cloudProgram.set("uGamma", sc1_gamma);
        scene->cloudProgram.set("uAtmosColor", sc1_atmosColor);
    }
    if (SCENE == 3) {
        Scene03* scene = (Scene03*)current_scene.get();
        scene->program.set("uRoughness", sc3_roughness);
        scene->debugPos = vec3(sc3_debugPosX, sc3_debugPosY, sc3_debugPosZ);
    }

    int scene_return = current_scene->render(FRAME, current_time, camera, DEBUG_MODE);

    prev_scene = SCENE;
    prev_time = time;

    if (scene_return != 0)
        SCENE = scene_return; // Rückgabe = 0:  Keine Änderung;   sonst neue Scene
}

void MainApp::switchScene() {
    FRAME = 0;
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    switch (SCENE) {
    case 0:
        current_scene = std::make_shared<TestScene>();
        break;
    case 1:
        // current_scene = scene_01;
        current_scene = std::make_shared<Scene01>();
        break;
    case 2:
        // current_scene = scene_02;
        current_scene = std::make_shared<Scene02>();
        break;
    case 3:
        // current_scene = scene_03;
        current_scene = std::make_shared<Scene03>();
        break;
    default:
        // current_scene = scene_01;
        current_scene = std::make_shared<TestScene>();
    }
    current_scene->setCameraPath(camera);
}

/*
 *  NOTE: Keyboard-Clicks  ->  Nutzung in Readme erklärt
 */
void MainApp::keyCallback(Key key, Action action) {
    if (key == Key::ESC && action == Action::PRESS)
        App::close();
    else if (key == Key::COMMA && action == Action::PRESS)
        App::imguiEnabled = !App::imguiEnabled;
    // else if (key == Key::LEFT_SHIFT && action == Action::PRESS) std::cout << "HEHEHEHA" <<
    // std::endl;
    else if (key == Key::BACKSPACE && action == Action::PRESS)
        reset_time_in_scene();

    else if (key == Key::W && action == Action::PRESS)
        wasdCallback(vec3(0.0f, 0.0f, 1.0f));
    else if (key == Key::S && action == Action::PRESS)
        wasdCallback(vec3(0.0f, 0.0f, -1.0f));
    else if (key == Key::D && action == Action::PRESS)
        wasdCallback(vec3(1.0f, 0.0f, 0.0f));
    else if (key == Key::A && action == Action::PRESS)
        wasdCallback(vec3(-1.0f, 0.0f, 0.0f));
    else if (key == Key::LEFT_SHIFT && action == Action::PRESS)
        wasdCallback(vec3(0.0f, -1.0f, 0.0f));
    else if (key == Key::SPACE && action == Action::PRESS)
        wasdCallback(vec3(0.0f, 1.0f, 0.0f));
    else if (key == Key::W && (action == Action::PRESS || action == Action::REPEAT))
        wasdCallback(vec3(0.0f, 0.0f, 1.0f));
    else if (key == Key::S && (action == Action::PRESS || action == Action::REPEAT))
        wasdCallback(vec3(0.0f, 0.0f, -1.0f));
    else if (key == Key::D && (action == Action::PRESS || action == Action::REPEAT))
        wasdCallback(vec3(1.0f, 0.0f, 0.0f));
    else if (key == Key::A && (action == Action::PRESS || action == Action::REPEAT))
        wasdCallback(vec3(-1.0f, 0.0f, 0.0f));
    else if (key == Key::LEFT_SHIFT && (action == Action::PRESS || action == Action::REPEAT))
        wasdCallback(vec3(0.0f, -1.0f, 0.0f));
    else if (key == Key::SPACE && (action == Action::PRESS || action == Action::REPEAT))
        wasdCallback(vec3(0.0f, 1.0f, 0.0f));
}

/*
 *  TODO: Mouse-Clicks umsetzen
 */
void MainApp::clickCallback(Button button, Action action, Modifier modifier) {
    // Mouse
}

/*
 *  TODO: GUI umsetzen. GUI ist normalerweise ausgeschaltet, soll Debug-Funktionen zur Verfügung
 * stellen.
 */
void MainApp::buildImGui() {
    ImGui::StatisticsWindow(App::delta, App::resolution);
    ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("Position:  (%f|%f|%f)", camera.cartesianPosition[0], camera.cartesianPosition[1],
                camera.cartesianPosition[2]);
    ImGui::Text("Spherical: (%f|%f|%f)", camera.sphericalPosition[0], camera.sphericalPosition[1],
                camera.sphericalPosition[2]);
    ImGui::Text("Target:    (%f|%f|%f)", camera.target[0], camera.target[1], camera.target[2]);

    ImGui::Text("Frame:  %u    \t\t|   Scene:  %u", FRAME, SCENE);
    ImGui::Text("Time:   %f \t| Scene-Start:   %f \t Global-Time:  %f", current_time,
                scene_start_time, time);
    if (ImGui::Button("Reset Time"))
        reset_time_in_scene();
    ImGui::Text("---------------------------------------------------");

    ImGui::Checkbox("Debug Mode", &DEBUG_MODE);
    ImGui::Checkbox("Play Animation", &ANIMATION_PLAYING);
    ImGui::RadioButton("Test scene", &SCENE, 0);
    ImGui::RadioButton("Scene 1", &SCENE, 1);
    ImGui::RadioButton("Scene 2", &SCENE, 2);
    ImGui::RadioButton("Scene 3", &SCENE, 3);
    if (SCENE == 1) {
        ImGui::SphericalSlider("sun", sc1_lightDir);
        if (ImGui::CollapsingHeader("Earth options")) {
            ImGui::SliderFloat("Land threshold", &sc1_landThreshold, 0.01, 1);
            ImGui::ColorPicker3("Ocean color", (float*)&sc1_oceanColor);
            ImGui::ColorPicker3("Land color", (float*)&sc1_landColor);
            ImGui::ColorPicker3("Mountain color", (float*)&sc1_mountainColor);
        }
        if (ImGui::CollapsingHeader("Cloud options")) {
            ImGui::SliderFloat("alpha", &sc1_alpha, -1.57, 1.57);
            ImGui::SliderFloat("beta", &sc1_beta, 0.0, 6.28);
            ImGui::SliderFloat("gamma", &sc1_gamma, -3.14, 3.14);
            ImGui::ColorPicker3("athmosphere color", (float*)&sc1_atmosColor);
        }
    }
    else if (SCENE == 3) {
        ImGui::SliderFloat("Roughness", &sc3_roughness, 0.0f, 1.0f);
        ImGui::SliderFloat("x", &sc3_debugPosX, -10.f, 10.f);
        ImGui::SliderFloat("y", &sc3_debugPosY, -10.f, 10.f);
        ImGui::SliderFloat("z", &sc3_debugPosZ, -10.f, 10.f);
    }
    ImGui::End();
}

void MainApp::reset_time_in_scene() {
    scene_start_time = time - floor(time);
    current_time = scene_start_time;
}

void MainApp::scrollCallback(float amount) { camera.zoom(amount); }

void MainApp::moveCallback(const vec2& movement, bool leftButton, bool rightButton,
                           bool middleButton) {
    // std::cout << std::to_string(movement.x) << "|" << std::to_string(movement.y) << std::endl;
    if (rightButton | middleButton)
        camera.rotate(movement * 0.01f);
}

void MainApp::resizeCallback(const vec2& resolution) { camera.resize(resolution.x / resolution.y); }

void MainApp::wasdCallback(const vec3& movement) { camera.move_target(movement); }
