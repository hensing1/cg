#include "csv.hpp"
#include "glm/fwd.hpp"
#include "mesh.hpp"
#include "objparser.hpp"
#include "scene.hpp"
#include <algorithm>
#include <cmath>
//#include <stb_image.h>
#include <iostream>

Scene01::Scene01(Program& program, MovableCamera& camera) {
    program.load("earth.vert", "earth.frag");
    HDS icoHDS = generate_icosahedron();
    // HDS icoHDS = generateTrongle();
    int subidivisions = 6;
    for (int i = 0; i < subidivisions; i++) {
        icoHDS.loop_subdivision();
    // icoHDS.print();
    }

    HDS::VaoData sphere_data = icoHDS.generate_vao_data();
    elevation_map = load_elevation_map(); // std::vector<std::vector<float>>

    size_t width = elevation_map[0].size();
    size_t height = elevation_map.size();

    calculate_texture_coordinates(sphere_data.vertices);

    // internal format: GL_R32F
    // base format: GL_RED

    std::vector<float> contiguous_elevation_map;
    contiguous_elevation_map.reserve(width * height);

    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            contiguous_elevation_map[y * width + x] = elevation_map[height - 1 - y][width - 1 - x];
        }
    }

    glGenTextures(1, &textureHandle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureHandle);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, width, height, 0, GL_RED, GL_FLOAT,
                 contiguous_elevation_map.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //stbi_set_flip_vertically_on_load(true);

    earth = Mesh();
    earth.load(sphere_data.vertices, sphere_data.indices);
}

int Scene01::render(int frame, float time, Program& program, MovableCamera& camera, bool DEBUG) {
    camera.updateIfChanged();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    vec3 pos = vec3(0, 0, 0);
    mat4 worldToClip = camera.projectionMatrix * camera.viewMatrix;
    this->drawMesh(2.f, pos, program, earth, worldToClip);

    return 0;
}

void Scene01::calculate_texture_coordinates(std::vector<Mesh::VertexPCN>& sphere_vertices) {
    float pi = glm::pi<float>();
    for (auto& vertex : sphere_vertices) {
        float latitude = asin(vertex.position.y / length(vertex.position));

        vec2 pointOnLatitudeCircle = vec2{vertex.position.x, vertex.position.z};
        float longitude = length(pointOnLatitudeCircle) == 0 ? 0
                          : vertex.position.z >= 0
                              ? acos(vertex.position.x / length(pointOnLatitudeCircle))
                              : -acos(vertex.position.x / length(pointOnLatitudeCircle));

        vertex.texCoord = vec2{(pi + longitude) / (2 * pi), (pi / 2 + latitude) / pi};
    }
}

std::vector<std::vector<float>> Scene01::load_elevation_map() {
    auto values = CSV::read_csv("textures/elevation.csv");

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

    float newOceanHeight = min - 50.0f;

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

Scene01::~Scene01() {}
