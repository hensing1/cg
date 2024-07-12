#include "mesh.hpp"
#include <memory>

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
    };
    struct Face {
        std::weak_ptr<Halfedge> edge;
    };
    struct Node {
        unsigned int id;
        glm::vec3 position;
        std::vector<std::weak_ptr<Halfedge>> outgoing;
    };

    struct VaoData {
        std::vector<Mesh::VertexPCN> vertices;
        std::vector<unsigned int> indices;
    };

    HDS();
    HDS(const std::vector<glm::vec3>& vertices, const std::vector<unsigned int>& indices);
    // void load_mesh(Mesh mesh);
    void load(const std::vector<glm::vec3>& vertices,
              const std::vector<unsigned int>& indices);

    void loop_subdivision();
    Mesh generate_mesh();
    VaoData generate_vao_data();
    void print();

  private:
    std::vector<std::shared_ptr<Node>> nodes;
    std::vector<std::shared_ptr<Halfedge>> edges;
    std::vector<std::shared_ptr<Face>> faces;

    void set_twins(std::vector<std::shared_ptr<Halfedge>>& edges);

    std::weak_ptr<Halfedge> find_halfedge(std::weak_ptr<Node> from, std::weak_ptr<Node> to);

    void make_triangle(std::vector<std::shared_ptr<Face>>& faceList,
                       std::vector<std::shared_ptr<Halfedge>>& edgeList, std::shared_ptr<Node> v1,
                       std::shared_ptr<Node> v2, std::shared_ptr<Node> v3);

    glm::vec3 intermediate(glm::vec3& v1, glm::vec3& v2);

    void subdivide();

    glm::vec3 average(unsigned int vertIndex, std::vector<std::shared_ptr<Node>>& nodes);

};
