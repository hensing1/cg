#pragma once

#include <glm/glm.hpp>
using namespace glm;

#include "framework/app.hpp"
#include "framework/app.hpp"
#include "framework/camera.hpp"
#include "framework/mesh.hpp"
#include "framework/gl/program.hpp"
#include "framework/imguiutil.hpp"

#include "classes/hermite.hpp"

class MainApp : public App {
   public:
    MainApp();

    void render_scene_01();
    void render_scene_02();

   protected:
    void init() override;
    void buildImGui() override;
    void render() override;
    void keyCallback(Key key, Action action) override;
    void clickCallback(Button button, Action action, Modifier modifier) override;
    void scrollCallback(float amount) override;
    void moveCallback(const vec2& movement, bool leftButton, bool rightButton, bool middleButton) override;
    void resizeCallback(const vec2& resolution) override;

   private:
    Camera camera;
    Program program;

    // NOTE: Globale Variablen
    int FRAME;
    int SCENE;
    bool DEBUG_MODE;
    bool ANIMATION_PLAYING;

    //  NOTE: Zu Testzwecken -> später entfernen
    Mesh donut;
    Mesh cube;
    Mesh sphere;

    std::vector<hermite_point> donut_path_points;
    std::vector<hermite_point> cube_path_points;
    std::vector<hermite_point> sphere_path_points;
    std::vector<quintic_hermite_point> smooth_sphere_path_points;

    Hermite donut_path;
    Hermite cube_path;
    Hermite sphere_path;
    QuinticHermite smooth_sphere_path;
};
