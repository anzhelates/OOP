#pragma once

#include "Graph.h"
#include <list>
#include <vector>

template <typename TVertex, typename TEdge>
class AdjListGraph : public Graph<TVertex, TEdge> {
private:
    struct AdjEntry {
        int m_to;
        TEdge* m_edge;
        AdjEntry(int to, TEdge* edge) : m_to(to), m_edge(edge) {}
    };
    std::vector<std::list<AdjEntry>> m_adjList;

public:
    explicit AdjListGraph(bool directed = true) : Graph<TVertex, TEdge>(directed) {}

    int addVertex(TVertex* vertex) override {
        if (!vertex) return -1;
        int id = this->m_vertices.size();
        vertex->setId(id);
        this->m_vertices.push_back(vertex);
        m_adjList.resize(this->m_vertices.size());
        return id;
    }

    void addEdge(TEdge* edge) override {
        if (!edge || !edge->getSource() || !edge->getDestination()) return;
        this->m_edges.push_back(edge);
        int from = edge->getSource()->getId();
        int to = edge->getDestination()->getId();
        m_adjList[from].push_back(AdjEntry(to, edge));
        if (!this->m_directed) {
            m_adjList[to].push_back(AdjEntry(from, edge));
        }
    }


    std::vector<int> getNeighbors(int id) const override {
        std::vector<int> neighbors;
        if (id >= 0 && id < m_adjList.size()) {
            for (const auto& entry : m_adjList[id]) {
                neighbors.push_back(entry.m_to);
            }
        }
        return neighbors;
    }

    TEdge* getEdge(int fromId, int toId) const override {
        if (fromId >= 0 && fromId < m_adjList.size()) {
            for (const auto& entry : m_adjList[fromId]) {
                if (entry.m_to == toId) return entry.m_edge;
            }
        }
        return nullptr;
    }
};