#include "scene.hpp"

#include "classes/operations.hpp"

TestScene::TestScene(MovableCamera& camera) {
    program.load("Scene2.vert", "Scene2.frag");
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
        hermite_point{vec3(0.0f, glm::pi<float>(), 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        hermite_point{vec3(0.0f, 0.0f, glm::pi<float>()), vec3(0.0f, 0.0f, 0.0f)},
        hermite_point{vec3(glm::pi<float>(), 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
    };
    rotation_path = Hermite(&rotation_path_points);

    camera_path_points = {
        //quintic_hermite_point{vec3(10.0f, glm::pi<float>()/2, 0.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f)},
        //quintic_hermite_point{vec3(26.0f, 0.0f, glm::pi<float>()/2), vec3(0.0f, glm::pi<float>() / 4, 0.0f), vec3(0.0f)},
        //quintic_hermite_point{vec3(12.0f, -glm::pi<float>()/2, 0.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f)},
        quintic_hermite_point{vec3(0.02f, 0.0f, 0.0f), vec3(0.0f, glm::pi<float>()/4, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(0.02f, glm::pi<float>(), 0.0f), vec3(0.0f, glm::pi<float>()/4, 0.0f), vec3(0.0f, -glm::pi<float>()/8, 0.0f)},
        quintic_hermite_point{vec3(0.02f, glm::pi<float>()*2, glm::pi<float>()), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(0.02f, glm::pi<float>(), glm::pi<float>()*2), vec3(0.0f, -glm::pi<float>()/4, 0.0f), vec3(0.0f, glm::pi<float>()/8, 0.0f)},
        quintic_hermite_point{vec3(0.02f, 0.0f, glm::pi<float>()), vec3(0.0f, -glm::pi<float>()/4, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
    };
    
    quintic_hermite_point starter_point = {vec3(0.2f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f)};
    std::vector<vec3> point_list = {
        vec3(0.02f, glm::pi<float>(), 0.0f),
        vec3(0.02f, glm::pi<float>()*2, glm::pi<float>()),
        vec3(0.02f, glm::pi<float>(), glm::pi<float>()*2),
        vec3(0.02f, 0.0f, glm::pi<float>()),
    };

    camera.setPath(QuinticHermite(&camera_path_points));
    //camera.setPath(QuinticHermite(starter_point, point_list));
    view_path_points = {
        quintic_hermite_point{vec3(0.0f, 10.0f, 0.0f), vec3(0.0f, -10.0f, 0.0f), vec3(0.0f, -2.0f, 0.0f)},
        quintic_hermite_point{vec3(0.0f, 5.0f, 0.0f), vec3(0.0f, -5.0f, 0.0f),vec3(0.0f, -2.0f, 0.0f)},
        quintic_hermite_point{vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f),vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(0.0f, -5.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f),vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f),vec3(0.0f, 0.0f, 0.0f)},
    };
    camera.setViewDirPath(QuinticHermite(&view_path_points));

}

int TestScene::render(int frame, float time, MovableCamera& camera, bool DEBUG) {
    if (!DEBUG) {
        camera.setViewDirAlongSpline(time / 2);
        camera.setPosAlongSpline(time / 2);
    }
    camera.updateIfChanged();

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
             Operations::get_rotation_matrix(rotation_path.evaluateSplineAllowLoop(time*1.5)) );

    return 0;
}

TestScene::~TestScene() {
    // donut.load(nullptr);
}
