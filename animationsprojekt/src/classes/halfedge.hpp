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

    struct Halfedge {
        std::weak_ptr<Node> start;
        std::weak_ptr<Halfedge> prev;
        std::weak_ptr<Halfedge> next;
        std::weak_ptr<Halfedge> twin;
        std::weak_ptr<Face> face;
        bool divided;
    };
    struct Face {
        std::weak_ptr<Halfedge> edge;
    };
    struct Node {
        unsigned int id;
        Mesh::VertexPCN vertex;
        // std::unordered_set<unsigned int> neighbors;
        std::vector<std::weak_ptr<Halfedge>> outgoing;
    };

    HDS();
    HDS(const std::vector<Mesh::VertexPCN>& vertices, const std::vector<unsigned int>& indices);
    // void load_mesh(Mesh mesh);
    void load(const std::vector<Mesh::VertexPCN>& vertices,
              const std::vector<unsigned int>& indices);

    void subdivide();
    Mesh generate_mesh();

  private:
    std::vector<std::shared_ptr<Node>> nodes;
    std::vector<std::shared_ptr<Halfedge>> edges;
    std::vector<std::shared_ptr<Face>> faces;

    void set_twins(std::vector<std::shared_ptr<Halfedge>> edges);

    std::weak_ptr<Halfedge> find_halfedge(std::weak_ptr<Node> from, std::weak_ptr<Node> to);

    void make_triangle(std::vector<std::shared_ptr<Face>>& faceList,
                       std::vector<std::shared_ptr<Halfedge>>& edgeList, std::shared_ptr<Node> v1,
                       std::shared_ptr<Node> v2, std::shared_ptr<Node> v3);

    Mesh::VertexPCN intermediate(Mesh::VertexPCN& v1, Mesh::VertexPCN& v2);
};
