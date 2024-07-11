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

  private:
    // NOTE: Globale Variablen
    MovableCamera camera;
    std::unique_ptr<Scene> current_scene;

    int FRAME;
    int SCENE;
    int prev_scene;
    bool DEBUG_MODE;
    bool ANIMATION_PLAYING;

    float prev_time;
    float scene_start_time;

    vec3 oceanColor;
    vec3 landColor;
};
