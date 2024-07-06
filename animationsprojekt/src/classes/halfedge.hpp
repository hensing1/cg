#include "mesh.hpp"
#include <memory>
#include <unordered_set>

/**
 * Halbkanten-Datenstruktur
 */
class HDS {
    public:
        struct Halfedge;
        struct Face;
        struct Node;

        struct Halfedge{
            std::weak_ptr<Node> start;
            std::weak_ptr<Halfedge> prev;
            std::weak_ptr<Halfedge> next;
            std::weak_ptr<Halfedge> twin;
            std::weak_ptr<Face> face;
        };
        struct Face {
            std::weak_ptr<Halfedge> edge;
        };
        struct Node {
            Mesh::VertexPCN vertex;
            std::unordered_set<unsigned int> neighbors;
        };

        HDS();
        HDS(const std::vector<Mesh::VertexPCN>& vertices, const std::vector<unsigned int>& indices);
        // void load_mesh(Mesh mesh);
        void load(const std::vector<Mesh::VertexPCN>& vertices, const std::vector<unsigned int>& indices);
        Mesh generate_mesh();
        HDS subdivide();
    private:
        std::vector<std::shared_ptr<Node>> nodes;
        std::vector<std::shared_ptr<Halfedge>> edges;
        std::vector<std::shared_ptr<Face>> faces;
        
};
