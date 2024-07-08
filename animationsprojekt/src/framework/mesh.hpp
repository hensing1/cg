#pragma once

#include <glm/glm.hpp>
#include <map>
#include <string>
#include <vector>

#include "gl/buffer.hpp"
#include "gl/vertexarray.hpp"

struct Vertex {
    glm::vec3 position;
    glm::vec2 uv;
    glm::vec3 normal;
};

struct Face {
    std::vector<Vertex> vertices;
    std::string material;
};

struct Material {
    glm::vec3 diffuse;
    glm::vec3 ambient;
    glm::vec3 specular;
    std::string diffuseTexture;
};

struct MeshData {
    std::vector<Face> faces;
    std::map<std::string, Material> materials;
};

const std::vector<float> FULLSCREEN_VERTICES = {
    -1.0f, -1.0f, 0.0f,
     3.0f, -1.0f, 0.0f,
    -1.0f,  3.0f, 0.0f,
};

const std::vector<unsigned int> FULLSCREEN_INDICES = {
    0, 1, 2,
};

class Mesh {
   public:
       MeshData data;
    /**
     * Vertex with 3 position components, 2 texture coordinate components, 3 normal vector components
     */
    struct VertexPCN {
        glm::vec3 position;
        glm::vec2 texCoord;
        glm::vec3 normal;
    };
    /**
     * Vertex with 3 position components, 2 texture coordinate components, 3 normal vector components, 3 tangent vector components
     */
    struct VertexPCNT {
        glm::vec3 position;
        glm::vec2 texCoord;
        glm::vec3 normal;
        glm::vec3 tangent;
    };

    void load(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
    void load(const std::vector<VertexPCN>& vertices, const std::vector<unsigned int>& indices);
    void load(const std::vector<VertexPCNT>& vertices, const std::vector<unsigned int>& indices);
    bool load(const std::string& filepath);
    void loadWithTangents(const std::string& filepath);
    void draw(Program& program);
    void draw(GLuint instances);
    
    unsigned int numIndices = 0;
    VertexArray vao;
    Buffer vbo;
    Buffer ebo;
};
