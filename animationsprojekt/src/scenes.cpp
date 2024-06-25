
#include "mainapp.hpp"

#include <glad/glad.h>
#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include "framework/app.hpp"
#include "framework/camera.hpp"
#include "framework/gl/program.hpp"
#include "framework/mesh.hpp"

#include "classes/hermite.hpp"

#include "config.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <string>
#include <glm/gtx/string_cast.hpp>





//  NOTE: Nur zu Testzwecken -> später entfernen
void drawMesh(float size, const vec3& pos, Program& program, Mesh& mesh, const mat4& worldToClip) {
    mat4 localToWorld = scale(translate(mat4(1.0f), pos), vec3(size));
    program.set("uLocalToWorld", localToWorld);
    program.set("uLocalToClip", worldToClip * localToWorld);
    program.bind();
    mesh.draw();
    srand(42); // Reset random seed for static colors
}





void MainApp::render_scene_01() {
    //  TODO: Szene 1
    camera.updateIfChanged();
    mat4 worldToClip = camera.projectionMatrix * camera.viewMatrix;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
    
    std::vector<hermite_point> donut_path_points = {
        hermite_point{vec3(0.0f, -14.0f, 0.0f), vec3(0.5f, 1.0f, 0.0f)},
        hermite_point{vec3(4.0f, 4.0f, 0.0f), vec3(-2.0f, 0.5f, 0.5f)},
        hermite_point{vec3(1.0f, 8.0f, 1.0f), vec3(-2.4f, 0.0f, 0.4f)},
        hermite_point{vec3(1.0f, 4.0f, 1.0f), vec3(-12.0f, -2.0f, 3.0f)},
        hermite_point{vec3(-6.0f, -3.0f, -4.0f), vec3(0.0f, -5.0f, 0.0f)},
    }; 
    Hermite donut_path(&donut_path_points);

    std::vector<hermite_point> cube_path_points = {
        hermite_point{vec3(10.0f, -4.0f, 0.0f), vec3(-20.0f, 23.0f, 0.0f)},
        hermite_point{vec3(-10.0f, 8.0f, 0.0f), vec3(0.0f, -5.0f, 18.0f)},
        hermite_point{vec3(-5.0f, -4.0f, -15.0f), vec3(-20.0f, 23.0f, 0.0f)},
    };
    Hermite cube_path(&cube_path_points);

    std::vector<hermite_point> sphere_path_points = {
        hermite_point{vec3(-20.0f, 0.0f, 0.0f), vec3(0.0f, 30.0f, 0.0f)},
        hermite_point{vec3(0.0f, 20.0f, 0.0f), vec3(40.0f, 0.0f, 0.0f)},
        hermite_point{vec3(20.0f, 0.0f, 0.0f), vec3(0.0f, -30.0f, 0.0f)},
        hermite_point{vec3(0.0f, -20.0f, 0.0f), vec3( -5.0f, 0.0f, 0.0f)},
    };
    Hermite sphere_path(&sphere_path_points);


    std::vector<quintic_hermite_point> smooth_sphere_path_points = {
        quintic_hermite_point{vec3(0.0f, 0.0f, -10.0f), vec3(0.0f, 15.0f, 15.0f), vec3(0.0f, 8.0f, 0.0f)},
        quintic_hermite_point{vec3(0.0f, 10.0f, 0.0f), vec3(0.0f, -15.0f, 15.0f), vec3(0.0f, 0.0f, 8.0f)},
        quintic_hermite_point{vec3(0.0f, 0.0f, 10.0f), vec3(0.0f, -15.0f, -15.0f), vec3(0.0f, -8.0f, 0.0f)},
        quintic_hermite_point{vec3(0.0f, -10.0f, 0.0f), vec3(0.0f, 15.0f, -15.0f), vec3(0.0f, 0.0f, -8.0f)},
    };
    QuinticHermite smooth_sphere_path(&smooth_sphere_path_points);




    // Draw Test-Meshes
    program.set("uColor", vec3(sin(float(FRAME)/14), 1.0f, cos(float(FRAME)/20)));
    drawMesh(0.25f, vec3(0.0f, 0.0f, 0.0f), program, cube, worldToClip);
    program.set("uColor", vec3(0.0f, 0.9f, 0.1f));
    drawMesh(0.85f, cube_path.evaluateSplineAllowLoop(float(FRAME) / 200), program, cube, worldToClip);
    program.set("uColor", vec3(0.0f, 0.3f, 0.0f));
    drawMesh(2.25f, sphere_path.evaluateSplineAllowLoop(float(FRAME) / 100), program, sphere, worldToClip);
    program.set("uColor", vec3(0.5f, 0.0f, 0.8f));
    drawMesh(5.05f, donut_path.evaluateSpline(float(FRAME) / 200), program, donut, worldToClip);
    program.set("uColor", vec3(0.8f, 0.0f, 0.8f));
    drawMesh(1.35f, smooth_sphere_path.evaluateSplineAllowLoop(float(FRAME) / 200), program, sphere, worldToClip);

    //std::cout << "----------------\nPOS:  " << to_string(smooth_sphere_path.evaluateSplineAllowLoop(float(FRAME) / 200)) << std::endl;

}

void MainApp::render_scene_02() {
    //  TODO: Szene 2
}



