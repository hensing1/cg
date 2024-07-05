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

    rotation_path_points = {
        hermite_point{vec3(0.0f, PI, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        hermite_point{vec3(0.0f, 0.0f, PI), vec3(0.0f, 0.0f, 0.0f)},
        hermite_point{vec3(PI, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
    };
    rotation_path = Hermite(&rotation_path_points);



}

void TestScene::render(int frame, float time, Program& program, MovableCamera& camera) {
    //camera.updateIfChanged();
    camera.setViewDirAlongSpline(time / 5);
    camera.setPosAlongSpline(time / 5);
    mat4 worldToClip = camera.projectionMatrix * camera.viewMatrix;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw Test-Meshes
    program.set("uColor",
                vec3(sin(time), 1.0f, cos(time / 3)));
    drawMesh(0.25f, vec3(0.0f, 0.0f, 0.0f), program, cube, worldToClip);
    program.set("uColor", vec3(0.0f, 0.9f, 0.1f));
    drawMesh(0.85f, cube_path.evaluateSplineAllowLoop(time / 4),
             program, cube, worldToClip);
    program.set("uColor", vec3(0.0f, 0.3f, 0.0f));
    drawMesh(2.25f, sphere_path.evaluateSplineAllowLoop(time / 18),
             program, sphere, worldToClip);
    program.set("uColor", vec3(cos(time / 17), 0.0f, sin(time / 10)));
    drawMesh(5.05f, donut_path.evaluateSpline(time / 3), program,
             donut, worldToClip);
    program.set("uColor", vec3(0.8f, 0.0f, 0.8f));
    drawMesh(1.35f,
             smooth_sphere_path.evaluateSplineAllowLoop(time / 2),
             program, sphere, worldToClip);
    program.set("uColor", vec3(0.3, 0.02, 0.2));
    drawMesh(2.85f, smooth_sphere_path.evaluateSplineAllowLoop(2.0 + time / 2), program, donut, worldToClip,
             operations.get_rotation_matrix(rotation_path.evaluateSplineAllowLoop(time*1.5)) );

}

void TestScene::init(MovableCamera &camera) {
    std::vector path_points = {
        quintic_hermite_point{vec3(10.0f, PI/2, 0.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f)},
        quintic_hermite_point{vec3(26.0f, 0.0f, PI/2), vec3(0.0f, PI / 4, 0.0f), vec3(0.0f)},
        quintic_hermite_point{vec3(12.0f, -PI/2, 0.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f)},
    };
    camera.setPath(QuinticHermite(&path_points));
    std::vector view_path_points = {
        hermite_point{vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        hermite_point{vec3(0.0f, 5.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        hermite_point{vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
    };
    camera.setViewDirPath(Hermite(&view_path_points));
}

TestScene::~TestScene() {
    // donut.load(nullptr);
}
