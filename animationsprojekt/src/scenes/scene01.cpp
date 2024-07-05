#include <iostream>
#include <string>

#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "scene.hpp"
#include "mesh.hpp"
#include "objparser.hpp"

Scene01::Scene01() {
    // ObjParser::parse("meshes/icosahedron.obj", earthVertices, earthIndices);
    std::vector<float> verts = {
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
    earthIndices = {
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

    earthVertices = std::vector<Mesh::VertexPCN>();
    for (size_t i = 0; i < verts.size() / 3; i++) {
        glm::vec3 vec = {
            verts[3 * i],
            verts[3 * i + 1],
            verts[3 * i + 2]
        };
        Mesh::VertexPCN vertex = {};
        vertex.position = vec;
        vertex.normal = glm::normalize(vec);
        earthVertices.push_back(vertex);
    }
    earth.load(earthVertices, earthIndices);
}

void Scene01::render(int frame, float time, Program& program, Camera& camera) {
    camera.updateIfChanged();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    vec3 pos = vec3(0, 0, 0);
    mat4 worldToClip = camera.projectionMatrix * camera.viewMatrix;
    this->drawMesh(2.f, pos, program, earth, worldToClip);

}

Scene01::~Scene01() {}
