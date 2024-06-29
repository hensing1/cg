#include "scene.hpp"

TestScene::TestScene() {
    donut.load("meshes/donut.obj");
    cube.load("meshes/cube.obj");
    sphere.load("meshes/highpolysphere.obj");

    donut_path_points = {
        hermite_point{vec3(0.0f, -14.0f, 0.0f), vec3(0.5f, 1.0f, 0.0f)},
        hermite_point{vec3(4.0f, 4.0f, 0.0f), vec3(-2.0f, 0.5f, 0.5f)},
        hermite_point{vec3(1.0f, 8.0f, 1.0f), vec3(-2.4f, 0.0f, 0.4f)},
        hermite_point{vec3(1.0f, 4.0f, 1.0f), vec3(-12.0f, -2.0f, 3.0f)},
        hermite_point{vec3(-6.0f, -3.0f, -4.0f), vec3(0.0f, -5.0f, 0.0f)},
    }; 
    donut_path = Hermite(&donut_path_points);

    cube_path_points = {
        hermite_point{vec3(10.0f, -4.0f, 0.0f), vec3(-20.0f, 23.0f, 0.0f)},
        hermite_point{vec3(-10.0f, 8.0f, 0.0f), vec3(0.0f, -5.0f, 18.0f)},
        hermite_point{vec3(-5.0f, -4.0f, -15.0f), vec3(-20.0f, 23.0f, 0.0f)},
    };
    cube_path = Hermite(&cube_path_points);

    sphere_path_points = {
        hermite_point{vec3(-20.0f, 0.0f, 0.0f), vec3(0.0f, 30.0f, 0.0f)},
        hermite_point{vec3(0.0f, 20.0f, 0.0f), vec3(40.0f, 0.0f, 0.0f)},
        hermite_point{vec3(20.0f, 0.0f, 0.0f), vec3(0.0f, -30.0f, 0.0f)},
        hermite_point{vec3(0.0f, -20.0f, 0.0f), vec3( -5.0f, 0.0f, 0.0f)},
    };
    sphere_path = Hermite(&sphere_path_points);


    smooth_sphere_path_points = {
        quintic_hermite_point{vec3(0.0f, 0.0f, -10.0f), vec3(0.0f, 15.0f, 15.0f), vec3(0.0f, 8.0f, 0.0f)},
        quintic_hermite_point{vec3(0.0f, 10.0f, 0.0f), vec3(0.0f, -15.0f, 15.0f), vec3(0.0f, 0.0f, 8.0f)},
        quintic_hermite_point{vec3(0.0f, 0.0f, 10.0f), vec3(0.0f, -15.0f, -15.0f), vec3(0.0f, -8.0f, 0.0f)},
        quintic_hermite_point{vec3(0.0f, -10.0f, 0.0f), vec3(0.0f, 15.0f, -15.0f), vec3(0.0f, 0.0f, -8.0f)},
    };
    smooth_sphere_path = QuinticHermite(&smooth_sphere_path_points);

}

void TestScene::render(int frame, Program& program, Camera& camera) {
    camera.updateIfChanged();
    mat4 worldToClip = camera.projectionMatrix * camera.viewMatrix;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw Test-Meshes
    program.set("uColor",
                vec3(sin(float(frame) / 14), 1.0f, cos(float(frame) / 20)));
    drawMesh(0.25f, vec3(0.0f, 0.0f, 0.0f), program, cube, worldToClip);
    program.set("uColor", vec3(0.0f, 0.9f, 0.1f));
    drawMesh(0.85f, cube_path.evaluateSplineAllowLoop(float(frame) / 200),
             program, cube, worldToClip);
    program.set("uColor", vec3(0.0f, 0.3f, 0.0f));
    drawMesh(2.25f, sphere_path.evaluateSplineAllowLoop(float(frame) / 100),
             program, sphere, worldToClip);
    program.set("uColor", vec3(0.5f, 0.0f, 0.8f));
    drawMesh(5.05f, donut_path.evaluateSpline(float(frame) / 200), program,
             donut, worldToClip);
    program.set("uColor", vec3(0.8f, 0.0f, 0.8f));
    drawMesh(1.35f,
             smooth_sphere_path.evaluateSplineAllowLoop(float(frame) / 200),
             program, sphere, worldToClip);
}

TestScene::~TestScene() {}
