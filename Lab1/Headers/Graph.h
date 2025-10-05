#pragma once

#include <vector>
#include "Vertex.h"
#include "Edge.h"

template <typename TVertex, typename TEdge>
class Graph {
    protected:
        std::vector<TVertex*> m_vertices;
        std::vector<TEdge*> m_edges;
        bool m_directed;
    public:
        explicit Graph(bool directed = true) : m_directed(directed) {}
        virtual ~Graph() {
            for (auto* vertex : m_vertices) { delete vertex; }
            for (auto* edge : m_edges) { delete edge; }
        }

        int addVertex(TVertex* vertex) {
            if (!vertex) return -1;
            int newId = m_vertices.size();
            vertex->setId(newId);
            m_vertices.push_back(vertex);
            return newId;
        }
        virtual void removeVertex(int id) = 0;
        virtual void addEdge(TEdge* edge) = 0;
        virtual void removeEdge(TEdge* edge) = 0;
        bool isDirected() const { return m_directed; }

        TVertex* getVertex(int id) const {
            if (id >= 0 && id < m_vertices.size()) return m_vertices[id];
            return nullptr;
        }
        const std::vector<TVertex*>& getVertices() const { return m_vertices; }
        const std::vector<TEdge*>& getEdges() const { return m_edges; }
        virtual double getWeight(int fromId, int toId, const Vehicle& vehicle) const = 0;
};