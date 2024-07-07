#include "halfedge.hpp"
#include <memory>
#include <stdexcept>

HDS::HDS(const std::vector<Mesh::VertexPCN>& vertices, const std::vector<unsigned int>& indices) {
    load(vertices, indices);
}

void HDS::load(const std::vector<Mesh::VertexPCN>& vertices,
               const std::vector<unsigned int>& indices) {
    nodes = std::vector<std::shared_ptr<Node>>(vertices.size());

    for (size_t i = 0; i < vertices.size(); i++) {
        auto node = std::make_shared<Node>();
        node->id = i;
        node->vertex = vertices[i];
        nodes.push_back(node);
    }

    for (size_t i = 0; i < indices.size() / 3; i++) { // drei Vertices pro Fläche
        make_triangle(faces, edges, nodes[3 * i], nodes[3 * i + 1], nodes[3 * i + 2]);
    }

    // Gegenkanten finden
    set_twins(edges);
}

void HDS::set_twins(std::vector<std::shared_ptr<Halfedge>> edges) {
    for (auto edge : edges) {
        auto startNode = edge->start;
        auto endNode = edge->next.lock()->start;
        edge->twin = find_halfedge(endNode, startNode);
    }
}

std::weak_ptr<HDS::Halfedge> HDS::find_halfedge(std::weak_ptr<Node> from, std::weak_ptr<Node> to) {
    for (auto edge : to.lock()->outgoing) {
        if (edge.lock()->next.lock()->start.lock()->id == from.lock()->id) {
            return edge;
        }
    }
    throw std::invalid_argument("No halfedge found"); // exception thrown because this should never happen in my use case
}

void HDS::make_triangle(std::vector<std::shared_ptr<Face>>& faceList,
                        std::vector<std::shared_ptr<Halfedge>>& edgeList,
                        std::shared_ptr<Node> node1,
                        std::shared_ptr<Node> node2,
                        std::shared_ptr<Node> node3) {

    auto face = std::make_shared<Face>();

    auto halfedge1 = std::make_shared<Halfedge>(),
         halfedge2 = std::make_shared<Halfedge>(),
         halfedge3 = std::make_shared<Halfedge>();

    face->edge = halfedge1;
    faceList.push_back(face);

    halfedge1->start = node1;
    halfedge2->start = node2;
    halfedge3->start = node3;

    halfedge1->prev = halfedge3;
    halfedge2->prev = halfedge1;
    halfedge3->prev = halfedge2;

    halfedge1->next = halfedge2;
    halfedge2->next = halfedge3;
    halfedge3->next = halfedge1;

    halfedge1->face = face;
    halfedge2->face = face;
    halfedge3->face = face;

    halfedge1->divided = false;
    halfedge2->divided = false;
    halfedge3->divided = false;

    edgeList.push_back(halfedge1);
    edgeList.push_back(halfedge2);
    edgeList.push_back(halfedge3);

    node1->outgoing.push_back(halfedge1);
    node2->outgoing.push_back(halfedge2);
    node3->outgoing.push_back(halfedge3);
}

void HDS::subdivide() {
    std::vector<std::shared_ptr<Face>> newFaces;
    std::vector<std::shared_ptr<Halfedge>> newEdges;

    auto halfwayNodes = std::vector<std::vector<unsigned int>>(nodes.size());
    for (unsigned int i = 0; i < halfwayNodes.size(); i++) {
        halfwayNodes[i] = std::vector<unsigned int>(nodes.size());
    }
    
    auto is_subdivided = [&](std::shared_ptr<Node> n1, std::shared_ptr<Node> n2) {
        return halfwayNodes[n1->id][n2->id] != 0;
    };

    auto make_halfway_node = [&](std::shared_ptr<Node> n1, std::shared_ptr<Node> n2) {
        auto newNode = std::make_shared<Node>();
        newNode->id = nodes.size();
        newNode->vertex = intermediate(n1->vertex, n2->vertex);
        nodes.push_back(newNode);
        halfwayNodes[n1->id][n2->id] = halfwayNodes[n2->id][n1->id] = newNode->id;
        return newNode;
    };

    for (const auto& face : faces) {

        std::shared_ptr<Node> node1 = face->edge.lock()->start.lock(),
                              node2 = face->edge.lock()->next.lock()->start.lock(),
                              node3 = face->edge.lock()->next.lock()->next.lock()->start.lock();

        // interpolate points
        auto node12 = is_subdivided(node1, node2) ?
                 nodes[halfwayNodes[node1->id][node2->id]] : make_halfway_node(node1, node2),
             node23 = is_subdivided(node2, node3) ?
                 nodes[halfwayNodes[node2->id][node3->id]] : make_halfway_node(node2, node3),
             node31 = is_subdivided(node3, node1) ?
                 nodes[halfwayNodes[node3->id][node1->id]] : make_halfway_node(node3, node1);


        make_triangle(newFaces, newEdges, node1, node12, node31);
        make_triangle(newFaces, newEdges, node12, node2, node23);
        make_triangle(newFaces, newEdges, node31, node23, node3);
        make_triangle(newFaces, newEdges, node12, node23, node31);
    }

    set_twins(newEdges);

    edges = newEdges;
    faces = newFaces;
}

Mesh::VertexPCN HDS::intermediate(Mesh::VertexPCN& node1, Mesh::VertexPCN& node2) {
    auto newVertex = Mesh::VertexPCN{};
    newVertex.position = node1.position + 0.5f * (node2.position - node1.position);
    return newVertex;
}
