#include "glm/fwd.hpp"
#include "scene.hpp"
#include "mesh.hpp"
#include "objparser.hpp"

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

Scene01::Scene01() {
    HDS icoHDS = generateIcosahedron();
    // HDS icoHDS = generateTrongle();
    int subidivisions = 4;
    for (int i = 0; i < subidivisions; i++) {
        icoHDS.loop_subdivision();
        // icoHDS.print();
    }
    
    earth = icoHDS.generate_mesh();
}

HDS Scene01::generateIcosahedron() {
    std::vector<float> icoVerts = {
        0.000000, -1.000000, 0.000000,
        0.723600, -0.447215, 0.525720,
        -0.276385, -0.447215, 0.850640,
        -0.894425, -0.447215, 0.000000,
        -0.276385, -0.447215, -0.850640,
        0.723600, -0.447215, -0.525720,
        0.276385, 0.447215, 0.850640,
        -0.723600, 0.447215, 0.525720,
        -0.723600, 0.447215, -0.525720,
        0.276385, 0.447215, -0.850640,
        0.894425, 0.447215, 0.000000,
        0.000000, 1.000000, 0.000000,
    };
    std::vector<unsigned int> icoInds = {
        0, 1, 2, 
        1, 0, 5, 
        0, 2, 3, 
        0, 3, 4, 
        0, 4, 5, 
        1, 5, 10, 
        2, 1, 6, 
        3, 2, 7, 
        4, 3, 8, 
        5, 4, 9, 
        1, 10, 6, 
        2, 6, 7, 
        3, 7, 8, 
        4, 8, 9, 
        5, 9, 10, 
        6, 10, 11, 
        7, 6, 11, 
        8, 7, 11, 
        9, 8, 11, 
        10, 9, 11, 
    };
    auto positions = std::vector<glm::vec3>();
    for (size_t i = 0; i < icoVerts.size() / 3; i++) {
        positions.push_back({
            icoVerts[3 * i],
            icoVerts[3 * i + 1],
            icoVerts[3 * i + 2]
        });
    }

    HDS icosahedron = HDS(positions, icoInds);
    return icosahedron;
}

void Scene01::render(int frame, float time, Program& program, Camera& camera) {
    camera.updateIfChanged();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    vec3 pos = vec3(0, 0, 0);
    mat4 worldToClip = camera.projectionMatrix * camera.viewMatrix;
    this->drawMesh(2.f, pos, program, earth, worldToClip);

}

Scene01::~Scene01() {}
