#pragma once

#include <glm/glm.hpp>
using namespace glm;

#include <vector>

#include "framework/app.hpp"
#include "framework/camera.hpp"
#include "framework/mesh.hpp"
#include "framework/gl/program.hpp"

class MainApp : public App {
   public:
    MainApp();

   protected:
    void init() override;
    void buildImGui() override;
    void render() override;
    void keyCallback(Key key, Action action) override;
    // void clickCallback(Button button, Action action, Modifier modifier) override;
    void scrollCallback(float amount) override;
    void moveCallback(const vec2& movement, bool leftButton, bool rightButton, bool middleButton) override;
    void resizeCallback(const vec2& resolution) override;

   private:
    Camera cam;
    Mesh mesh;
    Program meshShader;
    int uDebugView = 0;
    int uDistribution = 0;
    float uRoughness = 0.5f;
    float uMetallness = 0.0f;
    bool uUseOrenNayar = true;
    vec3 uAlbedo = vec3(1.022f, 0.782f, 0.344f);
    vec3 uLightDirection = normalize(vec3(1.0f, 1.0f, 1.0f));
    size_t meshID = 0;
};