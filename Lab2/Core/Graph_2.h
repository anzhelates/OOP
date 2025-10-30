#pragma once
#include <vector>
#include "Vertex_2.h"
#include "Edge_2.h"

class Graph_2 {
protected:
    std::vector<Vertex_2*> m_vertices;
    std::vector<Edge_2*> m_edges;
    bool m_directed;
    bool m_weighted;

public:
    explicit Graph_2(bool directed = true, bool weighted = false)
    : m_directed(directed), m_weighted(weighted) {}
    virtual ~Graph_2() {
        for (auto* vertex : m_vertices) { delete vertex; }
        for (auto* edge : m_edges) { delete edge; }
        m_vertices.clear();
        m_edges.clear();
    }
    virtual int addVertex(Vertex_2* vertex) = 0;
    virtual void addEdge(Edge_2* edge) = 0;
    virtual void removeVertex(int id) = 0;
    virtual void removeEdge(Edge_2* edge) = 0;

    bool isDirected() const { return m_directed; }
    bool isWeighted() const { return m_weighted; }
    int getNumberOfVertices() const { return static_cast<int>(m_vertices.size()); }

    Vertex_2* getVertexById(int id) const {
        if (id >= 0 && id < static_cast<int>(m_vertices.size())) {
            if (m_vertices[id] && m_vertices[id]->isActive()) {
                return m_vertices[id];
            }
        }
        return nullptr;
    }
    virtual std::vector<int> getNeighbors(int id) const = 0;
    virtual Edge_2* getEdge(int fromId, int toId) const = 0;
    virtual std::vector<Edge_2*> getEdgesFrom(int fromId) const = 0;
    const std::vector<Vertex_2*>& getVertices() const { return m_vertices; }
    const std::vector<Edge_2*>& getEdges() const { return m_edges; }
};