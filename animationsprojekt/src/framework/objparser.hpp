#include <string>
#include <vector>
#include "mesh.hpp"

namespace ObjParser {
    void parse(const std::string& filepath, std::vector<Mesh::VertexPCN>& vertices, std::vector<unsigned int>& indices);
    void parse(const std::string& filepath, std::vector<Mesh::VertexPCNT>& vertices, std::vector<unsigned int>& indices);
    bool parse(const std::string& filePath, Mesh& mesh);
    void parseMtl(const std::string& filename, std::map<std::string, Material>& materials);

    class ObjParser {
    public:
        bool parse(const std::string& filePath, Mesh& mesh);
    private:
        bool parseMtl(const std::string& filename, std::map<std::string, Material>& materials);
    };

}