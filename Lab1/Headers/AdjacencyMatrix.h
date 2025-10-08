#pragma once

#include "Graph.h"
#include <vector>

template <typename TVertex, typename TEdge>
class AdjMatrixGraph : public Graph<TVertex, TEdge> {
private:
    std::vector<std::vector<TEdge*>> m_adjMatrix;

public:
    explicit AdjMatrixGraph(bool directed = true) : Graph<TVertex, TEdge>(directed) {}

    int addVertex(TVertex* vertex) override {
        if (!vertex) return -1;
        int id = this->m_vertices.size();
        vertex->setId(id);
        this->m_vertices.push_back(vertex);

        m_adjMatrix.resize(this->m_vertices.size());
        for (auto& row : m_adjMatrix) {
            row.resize(this->m_vertices.size(), nullptr);
        }
        return id;
    }

    void addEdge(TEdge* edge) override {
        if (!edge || !edge->getSource() || !edge->getDestination()) return;
        int from = edge->getSource()->getId();
        int to = edge->getDestination()->getId();
        if (from < 0 || from >= (int)this->m_vertices.size()) return;
        if (to   < 0 || to   >= (int)this->m_vertices.size()) return;

        if (m_adjMatrix[from][to] != nullptr) return;
        this->m_edges.push_back(edge);
        m_adjMatrix[from][to] = edge;
        if (!this->m_directed) {
            m_adjMatrix[to][from] = edge;
        }
    }

    std::vector<int> getNeighbors(int id) const override {
        std::vector<int> neighbors;
        if (id >= 0 && id < m_adjMatrix.size()) {
            for (int i = 0; i < m_adjMatrix[id].size(); ++i) {
                if (m_adjMatrix[id][i] != nullptr) {
                    neighbors.push_back(i);
                }
            }
        }
        return neighbors;
    }

    TEdge* getEdge(int fromId, int toId) const override {
        if (fromId >= 0 && fromId < m_adjMatrix.size() && toId >= 0 && toId < m_adjMatrix.size()) {
            return m_adjMatrix[fromId][toId];
        }
        return nullptr;
    }
};