#pragma once

#include <glm/glm.hpp>
using namespace glm;

#include "framework/app.hpp"
#include "framework/mesh.hpp"
#include "framework/camera.hpp"
#include "framework/gl/program.hpp"

class MainApp : public App {
   public:
    MainApp();

   protected:
    void init() override;
    void render() override;
    void buildImGui() override;
    void keyCallback(Key key, Action action) override;
    // void clickCallback(Button button, Action action, const vec2& position) override;
    void scrollCallback(float amount) override;
    void moveCallback(const vec2& movement, bool leftButton, bool rightButton, bool middleButton) override;
    void resizeCallback(const vec2& resolution) override;

   private:
    Program program;
    Mesh mesh;
    Camera camera;
    unsigned int uView = 0;
    vec3 uLightDir = normalize(vec3(1.0));
    vec3 uLightColor = vec3(1.0);
    float uNear = 0.1;
    float uFar = 100.0;
    float uSteps = 100.0;
    float uEpsilon = 0.0001;
    float uNormalEps = 0.0001;
};