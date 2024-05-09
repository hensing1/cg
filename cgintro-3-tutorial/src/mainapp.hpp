#pragma once

#include <glm/glm.hpp>
using namespace glm;

#include "framework/app.hpp"
#include "framework/camera.hpp"
#include "framework/mesh.hpp"
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
    Camera camera;
    Mesh mesh;
    Program program;
    vec3 lightDir = normalize(vec3(1.0f, 1.0f, 1.0f));
    vec3 lightColor = vec3(1.0f);
    bool showNormals = false;
};