#include "objparser.hpp"

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <tiny_obj_loader.h>

using namespace glm;

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <stdexcept>
#include <filesystem>
#include <sstream>
#include <fstream>
#include "mesh.hpp"
#include "common.hpp"

////////////////////// Obj loading without tangents //////////////////////

/* Define comparison operator for use with std::unordered_map */
bool operator==(const Mesh::VertexPCN& v1, const Mesh::VertexPCN& v2) {
    return v1.position == v2.position && v1.texCoord == v2.texCoord && v1.normal == v2.normal;
}

/* Define hash function for use with std::unordered_map */
template<>
struct std::hash<Mesh::VertexPCN>
{
    std::size_t operator()(const Mesh::VertexPCN& vertex) const noexcept
    {
        size_t seed = 0;
        Common::hash_combine(seed, vertex.position, vertex.texCoord, vertex.normal);
        return seed;
    }
};

void ObjParser::parse(const std::string& filepath, std::vector<Mesh::VertexPCN>& vertices, std::vector<unsigned int>& indices) {
    // Parse OBJ file
    std::string rawobj = Common::readFile(filepath);
    tinyobj::ObjReader reader;
    tinyobj::ObjReaderConfig reader_config;
    reader_config.triangulate = true;
    if (!reader.ParseFromString(rawobj, "", reader_config))
        throw std::runtime_error("Failed to load OBJ file \"" + filepath + "\": " + reader.Error());
    if (!reader.Warning().empty())
        std::cout << "Warning loading OBJ file \"" << filepath << "\": " << reader.Warning() << std::endl;
    
    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();

    size_t predictedNumVertices = attrib.vertices.size() / 3;
    size_t predictedNumIndices = predictedNumVertices * 2 * 3; // Euler's polyhedron theorem

    std::unordered_map<Mesh::VertexPCN, uint32_t> uniqueVertices;
    uniqueVertices.reserve(predictedNumVertices);
    vertices.reserve(predictedNumVertices);
    indices.reserve(predictedNumIndices);

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            Mesh::VertexPCN vertex;
            
            vertex.position = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            if (index.texcoord_index >= 0) {
                vertex.texCoord = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    attrib.texcoords[2 * index.texcoord_index + 1]
                };
            }

            if (index.normal_index >= 0) {
                vertex.normal = {
                    attrib.normals[3 * index.normal_index + 0],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2]
                };
            }

            if (uniqueVertices.count(vertex) == 0) {
                uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                vertices.push_back(vertex);
            }

            indices.push_back(uniqueVertices[vertex]);
        }
    }
}

/////////////////////// Obj loading with tangents ///////////////////////

/* Define comparison operator for use with std::unordered_map */
bool operator==(const Mesh::VertexPCNT& v1, const Mesh::VertexPCNT& v2) {
    return v1.position == v2.position && v1.texCoord == v2.texCoord && v1.normal == v2.normal;
}

/* Define hash function for use with std::unordered_map */
template<>
struct std::hash<Mesh::VertexPCNT>
{
    std::size_t operator()(const Mesh::VertexPCNT& vertex) const noexcept
    {
        size_t seed = 0;
        Common::hash_combine(seed, vertex.position, vertex.texCoord, vertex.normal);
        return seed;
    }
};

void ObjParser::parse(const std::string& filepath, std::vector<Mesh::VertexPCNT>& vertices, std::vector<unsigned int>& indices) {
    // Parse OBJ file
    std::string rawobj = Common::readFile(filepath);
    tinyobj::ObjReader reader;
    tinyobj::ObjReaderConfig reader_config;
    reader_config.triangulate = true;
    if (!reader.ParseFromString(rawobj, "", reader_config))
        throw std::runtime_error("Failed to load OBJ file \"" + filepath + "\": " + reader.Error());
    if (!reader.Warning().empty())
        std::cout << "Warning loading OBJ file \"" << filepath << "\": " << reader.Warning() << std::endl;
    
    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();

    size_t predictedNumVertices = attrib.vertices.size() / 3;
    size_t predictedNumIndices = predictedNumVertices * 2 * 3; // Euler's polyhedron theorem

    std::unordered_map<Mesh::VertexPCNT, uint32_t> uniqueVertices;
    uniqueVertices.reserve(predictedNumVertices);
    vertices.reserve(predictedNumVertices);
    indices.reserve(predictedNumIndices);

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            Mesh::VertexPCNT vertex{};
            
            vertex.position = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            if (index.texcoord_index >= 0) {
                vertex.texCoord = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    attrib.texcoords[2 * index.texcoord_index + 1]
                };
            }

            if (index.normal_index >= 0) {
                vertex.normal = {
                    attrib.normals[3 * index.normal_index + 0],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2]
                };
            }

            vertex.tangent = vec3(0.0f);

            if (uniqueVertices.count(vertex) == 0) {
                uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                vertices.push_back(vertex);
            }

            indices.push_back(uniqueVertices[vertex]);
        }
    }

    std::vector<float> triangleCount(vertices.size());

    // Loop over triangles
    for (uint i = 0; i < indices.size(); i += 3) {
        // Get triangle vertices
        uint index0 = indices[i + 0u];
        uint index1 = indices[i + 1u];
        uint index2 = indices[i + 2u];
        Mesh::VertexPCNT& v0 = vertices[index0];
        Mesh::VertexPCNT& v1 = vertices[index1];
        Mesh::VertexPCNT& v2 = vertices[index2];

        vec3 delta_pos1 = v1.position - v0.position;
        vec3 delta_pos2 = v2.position - v0.position;

        vec2 delta_uv1 = v1.texCoord - v0.texCoord;
        vec2 delta_uv2 = v2.texCoord - v0.texCoord;

        float r = 1.0f / (delta_uv1.x * delta_uv2.y - delta_uv1.y * delta_uv2.x);
        vec3 tangent = (delta_pos1 * delta_uv2.y - delta_pos2 * delta_uv1.y) * r;

        v0.tangent += tangent;
        v1.tangent += tangent;
        v2.tangent += tangent;

        // Increment the triangle count for each vertex
        triangleCount[index0]++;
        triangleCount[index1]++;
        triangleCount[index2]++;
    }

    // Average tangents and bitangents per vertex
    for (uint i = 0; i < vertices.size(); i++) {
        if (triangleCount[i] > 0.0f) vertices[i].tangent /= triangleCount[i];
    }
}
bool ObjParser::parse(const std::string& filePath, Mesh& mesh) {
    std::ifstream objFile(filePath);
    std::string mtlFilePath;

    if (!objFile.is_open()) {
        std::cerr << "Failed to open OBJ file: " << filePath << std::endl;
        return false;
    }

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    std::string currentMaterial;

    std::string line;
    while (std::getline(objFile, line)) {
        std::istringstream ss(line);
        std::string keyword;
        ss >> keyword;

        if (keyword == "mtllib") {
            ss >> mtlFilePath;
        }
        else if (keyword == "v") {
            glm::vec3 vertex;
            ss >> vertex.x >> vertex.y >> vertex.z;
            vertices.push_back(vertex);
        }
        else if (keyword == "vt") {
            glm::vec2 uv;
            ss >> uv.x >> uv.y;
            uvs.push_back(uv);
        }
        else if (keyword == "vn") {
            glm::vec3 normal;
            ss >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);
        }
        else if (keyword == "usemtl") {
            ss >> currentMaterial;
        }
        else if (keyword == "f") {
            std::string vertexData;
            Face face;
            while (ss >> vertexData) {
                std::istringstream vs(vertexData);
                std::string v, t, n;
                std::getline(vs, v, '/');
                std::getline(vs, t, '/');
                std::getline(vs, n, '/');
                Vertex vertex;
                vertex.position = vertices[std::stoi(v) - 1];
                if (!t.empty()) vertex.uv = uvs[std::stoi(t) - 1];
                if (!n.empty()) vertex.normal = normals[std::stoi(n) - 1];
                face.vertices.push_back(vertex);
            }
            face.material = currentMaterial;
            mesh.data.faces.push_back(face);
        }
    }

    if (!mtlFilePath.empty()) {
        std::filesystem::path objPath(filePath);
        std::filesystem::path objDir = objPath.parent_path();
        std::filesystem::path mtlFullPath = objDir / mtlFilePath;
        parseMtl(mtlFullPath.string(), mesh.data.materials);
    }

    return true;
}
