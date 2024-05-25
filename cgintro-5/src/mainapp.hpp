#pragma once

#include <glm/glm.hpp>
using namespace glm;

#include <vector>

#include "framework/app.hpp"
#include "framework/camera.hpp"
#include "framework/mesh.hpp"
#include "framework/gl/program.hpp"
#include "framework/gl/buffer.hpp"

class MainApp : public App {
   public:
    MainApp();

   protected:
    void init() override;
    void render() override;
    void buildImGui() override;
    void keyCallback(Key key, Action action) override;
    void clickCallback(Button button, Action action, Modifier modifier) override;
    void scrollCallback(float amount) override;
    void moveCallback(const vec2& movement, bool leftButton, bool rightButton, bool middleButton) override;
    void resizeCallback(const vec2& resolution) override;

   private:
    Camera camera;
    Program program;
    Mesh mesh;
    std::vector<std::vector<vec3>> spline = {{vec3(0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f)}};
    float speed = 0.2f;
    int numberOfBalls = 10;
    bool closed = false;
};