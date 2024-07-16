#pragma once

#include <memory>

#include <glm/glm.hpp>
using namespace glm;

#include "framework/app.hpp"
#include "framework/camera.hpp"
#include "framework/gl/program.hpp"

#include "scenes/scene.hpp"
#include "classes/movable_camera.hpp"

class MainApp : public App {
  public:
    MainApp();

    void switchScene();

    void render_test_scene();
    void render_scene_01();
    void render_scene_02();
    void render_scene_03();
    void reset_time_in_scene();

  protected:
    void init() override;
    void buildImGui() override;
    void render() override;
    void keyCallback(Key key, Action action) override;
    void clickCallback(Button button, Action action,
                       Modifier modifier) override;
    void scrollCallback(float amount) override;
    void moveCallback(const vec2 &movement, bool leftButton, bool rightButton,
                      bool middleButton) override;
    void resizeCallback(const vec2 &resolution) override;
    void wasdCallback(const vec3 &movement);

  private:
    // NOTE: Globale Variablen
    MovableCamera camera;
    std::shared_ptr<Scene> current_scene;
    // std::shared_ptr<Scene01> scene_01;
    // std::shared_ptr<Scene02> scene_02;
    // std::shared_ptr<Scene03> scene_03;

    int FRAME;
    int SCENE;
    int prev_scene;
    bool DEBUG_MODE;
    bool ANIMATION_PLAYING;

    float prev_time;
    float current_time;
    float scene_start_time;

    // Szene 1
    vec3 sc1_lightDir;

    vec3 sc1_oceanColor;
    vec3 sc1_landColor;
    vec3  sc1_mountainColor;
    float sc1_landThreshold;

    float sc1_epsilon;
    float sc1_alpha;
    float sc1_beta;
    float sc1_gamma;
    // Szene 3
    float sc3_roughness;
};
