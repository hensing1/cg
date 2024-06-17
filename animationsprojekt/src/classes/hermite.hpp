
#pragma once

#include <glm/glm.hpp>
using namespace glm;

#include "framework/app.hpp"
#include "framework/app.hpp"
#include "framework/camera.hpp"
#include "framework/mesh.hpp"
#include "framework/gl/program.hpp"
#include "framework/imguiutil.hpp"

#include <vector>


struct hermite_point {
   vec3 pos;
   vec3 vel;
};

class Hermite {

public:
   Hermite(std::vector<hermite_point> *input);

   vec3 evaluateSpline(float t);
   vec3 evaluateSplineAllowLoop(float t);

private:
   std::vector<hermite_point> points;
   mat4 HERMITE_MATRIX;


};
