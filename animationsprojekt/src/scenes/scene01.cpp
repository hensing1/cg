#include "csv.hpp"
#include "gl/shader.hpp"
#include "glm/fwd.hpp"
#include "mesh.hpp"
#include "objparser.hpp"
#include "scene.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>

Scene01::Scene01() {

    program.load("earth.vert", "earth.frag");
    sphere.load("meshes/highpolysphere.obj");

    int num_subdivisions = 6;
    earth = generate_sphere(num_subdivisions);

    heightmapHandle = generate_and_apply_heightmap();

    cloudProgram.load("clouds.vert", "clouds.frag");
    cloudProgram.set("uEpsilon", 0.01f);
    cloudProgram.set("uEarthRadius", earthRadius);
    cloudCanvas.load(FULLSCREEN_VERTICES, FULLSCREEN_INDICES);

    camera_path_points = {
        quintic_hermite_point{vec3(1.5f, 0.0f, 0.0f), vec3(-0.f, 0.0f, 0.0f), vec3(-0.f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(1.5f, 0.0f, 0.0f), vec3(-0.f, 0.0f, 0.0f), vec3(-0.f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(1.5f, 0.0f, 0.0f), vec3(-0.f, 0.0f, 0.0f), vec3(-0.f, 0.0f, 0.0f)},
        // Flug auf die Erde zu
        quintic_hermite_point{vec3(1.5f, 0.0f, 0.0f), vec3(-0.3f, 0.0f, 0.0f), vec3(-0.4f, 0.0f, 0.0f)},
          //quintic_hermite_point{vec3(1.5f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), vec3(-0.3f, 0.0f, 0.0f)},
          //quintic_hermite_point{vec3(1.2f, PI/8, PI/16), vec3(-0.3f, PI/4, PI/8), vec3(0.0f, 0.0f, 0.0f)},
          //quintic_hermite_point{vec3(0.8f, PI/2, PI/4), vec3(-0.3f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
          //quintic_hermite_point{vec3(0.5f, PI/2, PI/4), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
          //quintic_hermite_point{vec3(0.5f, PI/2, PI/4), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
          //quintic_hermite_point{vec3(0.5f, PI/2, PI/4), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(1.0f, 0.191163f, 0.100583f), vec3(0.0f, 0.6f, 0.4f), vec3(0.0f, 0.5f, 0.3f)},
        quintic_hermite_point{vec3(1.0f, 0.881166f, 0.690583f), vec3(0.0f, 0.9f, 0.2f), vec3(0.0f, -0.2f, -0.2f)},
        // Zoom in die Kugel hinein
        quintic_hermite_point{vec3(0.99f, 1.711159f, 0.910796f), vec3(-0.400f, 0.0f, 0.0f), vec3(-0.2f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(0.60f, 1.711159f, 0.910796f), vec3(-0.350f, 0.0f, 0.0f), vec3( 0.310f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(0.30f, 1.711159f, 0.910796f), vec3(-0.225f, 0.0f, 0.0f), vec3( 0.235f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(0.15f, 1.711159f, 0.910796f), vec3(-0.125f, 0.0f, 0.0f), vec3( 0.135f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(0.10f, 1.711159f, 0.910796f), vec3(-0.050f, 0.0f, 0.0f), vec3( 0.085f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(0.05f, 1.711159f, 0.910796f), vec3(-0.050f, 0.0f, 0.0f), vec3( 0.0f, 0.0f, 0.0f)},
    };
    view_path_points = {
        quintic_hermite_point{vec3(0.0f, 0.0f, 10.0f), vec3(0.0f, 0.0f, -0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(0.0f, 0.0f, 10.0f), vec3(0.0f, 0.0f, -0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(0.0f, 0.0f, 10.0f), vec3(0.0f, 0.0f, -0.0f), vec3(0.0f, 0.0f, 0.0f)},
        // Flug auf die Erde zu
        quintic_hermite_point{vec3(0.0f, 0.0f, 10.0f), vec3(0.0f, 0.0f, -6.0f), vec3(0.0f, 0.0f, 0.0f)},
        //quintic_hermite_point{vec3(0.0f, 0.3f, 04.0f), vec3(0.0f, 0.3f, 0.0f), vec3(0.0f, 0.2f, 0.0f)},
        quintic_hermite_point{vec3(0.8f, 0.8f, 04.0f), vec3(0.65f, 0.3f, -3.0f), vec3(0.0f, 0.2f, 0.0f)},
        quintic_hermite_point{vec3(1.256274f, 1.366754f,  1.154455f), vec3(0.09f, 0.025f, -1.38f), vec3(0.0f, 0.0f, 0.0f)},
        // View-Point ändert sich nicht mehr
        quintic_hermite_point{vec3(1.241977f, 1.491430f, -0.177268f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(1.241977f, 1.491430f, -0.177268f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(1.241977f, 1.491430f, -0.177268f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(1.241977f, 1.491430f, -0.177268f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(1.241977f, 1.491430f, -0.177268f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(1.241977f, 1.491430f, -0.177268f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
    };

}

int Scene01::render(int frame, float time, MovableCamera& camera, bool DEBUG) {
    if (!DEBUG) {
        camera.setViewDirAlongSpline(time / 2);
        camera.setPosAlongSpline(time / 2);
    }
    bool cameraChanged = camera.updateIfChanged();

    glBindTexture(GL_TEXTURE_2D, heightmapHandle);

    // Erde rendern
    program.bind();
    glDisable(GL_BLEND);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    vec3 pos = vec3(0, 0, 0);
    mat4 worldToClip = camera.projectionMatrix * camera.viewMatrix;
    mat4 localToWorld = scale(translate(mat4(1.0f), pos), vec3(earthRadius));

    program.set("uLocalToWorld", localToWorld);
    program.set("uLocalToClip", worldToClip * localToWorld);
    earth.draw();

    // Wolken rendern
    cloudProgram.bind();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    cloudProgram.set("uTime", time);

    if (cameraChanged) {
        cloudProgram.set("uCameraPosition", camera.cartesianPosition);
        cloudProgram.set("uCameraMatrix", camera.cameraMatrix);
        cloudProgram.set("uAspectRatio", camera.aspectRatio);
        cloudProgram.set("uFocalLength", camera.focalLength);
        cloudProgram.set("uNear", camera.near);
        cloudProgram.set("uFar", camera.far);
    }
    cloudCanvas.draw();

    if (DEBUG) {
        render_debug_objects(program, worldToClip, camera.getViewDirAlongSpline(time / 2), camera.target);
    }

    if (time >= 15.3f) return 2;
    return 0;
}

void Scene01::render_debug_objects(Program& program, mat4 worldToClip, vec3 playerPosition, vec3 target) {
    program.set("uColor", vec3(0.65f, 0.00f, 0.4f));
    for (int i = 0; i < view_path_points.size(); i++) {
        drawMesh(0.10f, view_path_points[i].pos, program, sphere, worldToClip);
    }
    
    program.set("uColor", vec3(0.6f, 0.6f, 0.6f));
    this->drawMesh(0.1f, vec3(0.0f, 0.0f, 0.0f), program, sphere, worldToClip);
    program.set("uColor", vec3(1.0f, 0.0f, 0.0f));
    this->drawMesh(0.1f, vec3(0.12f, 0.0f, 0.0f), program, sphere, worldToClip);
    program.set("uColor", vec3(0.0f, 1.0f, 0.0f));
    this->drawMesh(0.1f, vec3(0.0f, 0.12f, 0.0f), program, sphere, worldToClip);
    program.set("uColor", vec3(0.0f, 0.0f, 1.0f));
    this->drawMesh(0.1f, vec3(0.0f, 0.0f, 0.12f), program, sphere, worldToClip);
    program.set("uColor", vec3(0.0f, 1.0f, 1.0f));
    this->drawMesh(0.03f, playerPosition, program, sphere, worldToClip);
    program.set("uColor", vec3(1.0f, 1.0f, 0.0f));
    this->drawMesh(0.01f, target, program, sphere, worldToClip);
}

Mesh Scene01::generate_sphere(int subdivisions) {
    HDS icoHDS = generate_icosahedron();

    for (int i = 0; i < subdivisions; i++) {
        icoHDS.loop_subdivision();
    }

    HDS::VaoData sphere_data = icoHDS.generate_vao_data();
    calculate_texture_coordinates(sphere_data.vertices);

    Mesh sphereMesh = Mesh();
    sphereMesh.load(sphere_data.vertices, sphere_data.indices);
    return sphereMesh;
}

GLuint Scene01::generate_and_apply_heightmap() {

    std::vector<std::vector<float>> elevation_map = load_elevation_map(); 

    size_t width = elevation_map[0].size();
    size_t height = elevation_map.size();

    std::vector<float> contiguous_elevation_map;
    contiguous_elevation_map.reserve(width * height);

    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            contiguous_elevation_map.push_back(elevation_map[height - 1 - y][width - 1 - x]); // openGL lädt die Texturdaten rückwärts
        }
    }

    GLuint handle;
    glGenTextures(1, &handle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, handle);
    
    // internal format: GL_R32F
    // base format: GL_RED, da nur eine float value (nur roter Kanal)

    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, width, height, 0, GL_RED, GL_FLOAT,
                 contiguous_elevation_map.data());
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return handle;

}

void Scene01::calculate_texture_coordinates(std::vector<Mesh::VertexPCN>& sphere_vertices) {
    for (auto& vertex : sphere_vertices) {
        float latitude = asin(vertex.position.y / length(vertex.position));

        vec2 pointOnLatitudeCircle = vec2{vertex.position.x, vertex.position.z};
        float longitude = length(pointOnLatitudeCircle) == 0 ? 0
                          : vertex.position.z >= 0
                              ? acos(vertex.position.x / length(pointOnLatitudeCircle))
                              : -acos(vertex.position.x / length(pointOnLatitudeCircle));

        float x = longitude / tau<float>() + 0.5f;
        float y = latitude / pi<float>() + 0.5f;

        vertex.texCoord = vec2{x, y};

    }
}

std::vector<std::vector<float>> Scene01::load_elevation_map() {
    auto values = CSV::read_csv("textures/elevation_hi_res.csv");

    float oceanMarker = 99999.0f;
    float max = -INFINITY;
    float min = INFINITY;

    for (size_t row = 0; row < values.size(); row++) {
        for (size_t col = 0; col < values[row].size(); col++) {
            if (values[row][col] == oceanMarker) {
                continue;
            }
            max = std::max(max, values[row][col]);
            min = std::min(min, values[row][col]);
        }
    }

    float newOceanHeight = min - (max - min) / 5;

    for (size_t row = 0; row < values.size(); row++) {
        for (size_t col = 0; col < values[row].size(); col++) {
            if (values[row][col] == oceanMarker) {
                values[row][col] = newOceanHeight;
            }
        }
    }

    // remap to interval [0,1]
    for (size_t row = 0; row < values.size(); row++) {
        for (size_t col = 0; col < values[row].size(); col++) {
            values[row][col] = (values[row][col] - newOceanHeight) / (max - newOceanHeight); 
        }
    }

    return values;
}

HDS Scene01::generate_icosahedron() {
    std::vector<float> icoVerts = {
        0.000000,  -1.000000, 0.000000,  0.723600, -0.447215, 0.525720,  -0.276385, -0.447215,
        0.850640,  -0.894425, -0.447215, 0.000000, -0.276385, -0.447215, -0.850640, 0.723600,
        -0.447215, -0.525720, 0.276385,  0.447215, 0.850640,  -0.723600, 0.447215,  0.525720,
        -0.723600, 0.447215,  -0.525720, 0.276385, 0.447215,  -0.850640, 0.894425,  0.447215,
        0.000000,  0.000000,  1.000000,  0.000000,
    };
    std::vector<unsigned int> icoInds = {
        0, 1, 2, 1, 0,  5, 0,  2,  3, 0, 3,  4,  0, 4,  5, 1, 5,  10, 2, 1,
        6, 3, 2, 7, 4,  3, 8,  5,  4, 9, 1,  10, 6, 2,  6, 7, 3,  7,  8, 4,
        8, 9, 5, 9, 10, 6, 10, 11, 7, 6, 11, 8,  7, 11, 9, 8, 11, 10, 9, 11,
    };
    auto positions = std::vector<glm::vec3>();
    for (size_t i = 0; i < icoVerts.size() / 3; i++) {
        positions.push_back({icoVerts[3 * i], icoVerts[3 * i + 1], icoVerts[3 * i + 2]});
    }

    HDS icosahedron = HDS(positions, icoInds);
    return icosahedron;
}

// HDS generateTrongle() {
//     std::vector<float> trongleVerts {
//         -1, 1, 0,
//         -1, -1, 0,
//         1, -1, 0
//     };
//
//     std::vector<unsigned int> trongleInds {
//         0,1,2,
//         2,1,0
//     };
//
//     auto positions = std::vector<glm::vec3>();
//     for (size_t i = 0; i < trongleVerts.size() / 3; i++) {
//         positions.push_back({
//             trongleVerts[3 * i],
//             trongleVerts[3 * i + 1],
//             trongleVerts[3 * i + 2]
//         });
//     }
//
//     HDS icosahedron = HDS(positions, trongleInds);
//     return icosahedron;
//
// }

Scene01::~Scene01() {
    glDisable(GL_BLEND);
}
