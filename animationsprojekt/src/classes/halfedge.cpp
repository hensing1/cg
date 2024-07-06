#include "halfedge.hpp"
#include <memory>

HDS::HDS(const std::vector<Mesh::VertexPCN>& vertices, const std::vector<unsigned int>& indices) {
    load(vertices, indices);
}

void HDS::load(const std::vector<Mesh::VertexPCN>& vertices, const std::vector<unsigned int>& indices) {
    nodes = std::vector<std::shared_ptr<Node>>(vertices.size());

    for (auto& vertex : vertices) {
        auto node = std::make_shared<Node>();
        node->vertex = vertex;
        node->neighbors = std::unordered_set<unsigned int>();
        nodes.push_back(node);
    }

    for (size_t i = 0; i < indices.size() / 3; i++) {  // drei Vertices pro Fläche
        auto face = std::make_shared<Face>();
        auto halfedge1 = std::make_shared<Halfedge>(),
             halfedge2 = std::make_shared<Halfedge>(),
             halfedge3 = std::make_shared<Halfedge>();
        halfedge1->face = face;
        halfedge2->face = face;
        halfedge3->face = face;

        halfedge1->next = halfedge2;
        halfedge2->next = halfedge3;
        halfedge3->next = halfedge1;

        halfedge1->prev = halfedge3;
        halfedge2->prev = halfedge1;
        halfedge3->prev = halfedge2;

        halfedge1->start = nodes[indices[3 * i]];
        halfedge2->start = nodes[indices[3 * i + 1]];
        halfedge3->start = nodes[indices[3 * i + 2]];

        nodes[indices[3 * i]]->neighbors.insert(indices[3 * i + 1]);
        nodes[indices[3 * i]]->neighbors.insert(indices[3 * i + 2]);
        nodes[indices[3 * i + 1]]->neighbors.insert(indices[3 * i]);
        nodes[indices[3 * i + 1]]->neighbors.insert(indices[3 * i + 2]);
        nodes[indices[3 * i + 2]]->neighbors.insert(indices[3 * i + 1]);
        nodes[indices[3 * i + 2]]->neighbors.insert(indices[3 * i + 2]);

        face->edge = halfedge1;
    }
}

HDS HDS::subdivide() {
    HDS subdivided = HDS();
    for (const auto& face : faces) {

    }
}
