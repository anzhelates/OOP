#pragma once

#include "Graph_2.h"
#include <list>
#include <vector>
#include <stack>

class AdjacencyList_2 : public Graph_2 {
private:
    struct AdjEntry {
        int m_to;
        Edge_2* m_edge;
        AdjEntry(int to, Edge_2* edge) : m_to(to), m_edge(edge) {}
    };

    std::vector<std::list<AdjEntry>> m_adjList;
    std::stack<int> m_freeIds;

public:
    explicit AdjacencyList_2(bool directed = true, bool weighted = false)
        : Graph_2(directed, weighted) {}

    ~AdjacencyList_2() override = default;

    int addVertex(Vertex_2* vertex) override {
        if (!vertex) return -1;

        int id;
        if (!m_freeIds.empty()) {
            id = m_freeIds.top();
            m_freeIds.pop();
            delete m_vertices[id];
            m_vertices[id] = vertex;
            m_adjList[id].clear();
        } else {
            id = static_cast<int>(this->m_vertices.size());
            this->m_vertices.push_back(vertex);
            m_adjList.resize(this->m_vertices.size());
        }

        vertex->setId(id);
        vertex->markActive();
        return id;
    }

    void addEdge(Edge_2* edge) override {
        if (!edge || !edge->getSource() || !edge->getDestination()) return;

        int from = edge->getSource()->getId();
        int to = edge->getDestination()->getId();

        if (from < 0 || from >= static_cast<int>(m_vertices.size()) ||
            to < 0 || to >= static_cast<int>(m_vertices.size())) {
            return;
        }

        if (!m_vertices[from]->isActive() || !m_vertices[to]->isActive()) {
            return;
        }

        edge->markActive();
        this->m_edges.push_back(edge);

        m_adjList[from].push_back(AdjEntry(to, edge));
        if (!this->m_directed) {
            m_adjList[to].push_back(AdjEntry(from, edge));
        }
    }

    void removeEdge(Edge_2* edge) override {
        if (!edge || !edge->getSource() || !edge->getDestination()) return;

        int from = edge->getFrom();
        int to = edge->getTo();

        if (from >= 0 && from < static_cast<int>(m_adjList.size())) {
            m_adjList[from].remove_if([edge](const AdjEntry& entry) {
                return entry.m_edge == edge;
            });
        }
        if (!this->m_directed && to >= 0 && to < static_cast<int>(m_adjList.size())) {
             m_adjList[to].remove_if([edge](const AdjEntry& entry) {
                return entry.m_edge == edge;
            });
        }
        edge->markInactive();
        m_edges.erase(std::remove(m_edges.begin(), m_edges.end(), edge), m_edges.end());
    }

    void removeVertex(int id) override {
        if (id < 0 || id >= static_cast<int>(this->m_vertices.size()) || !m_vertices[id]->isActive()) {
            return;
        }
        std::set<Edge_2*> edgesToRemove;

        for (const auto& entry : m_adjList[id]) {
            if (entry.m_edge) {
                edgesToRemove.insert(entry.m_edge);
            }
        }
        m_adjList[id].clear();

        for (int i = 0; i < static_cast<int>(m_adjList.size()); ++i) {
            if (i == id) continue;
            m_adjList[i].remove_if([id, &edgesToRemove](const AdjEntry& entry) {
                if (entry.m_to == id && entry.m_edge) {
                    edgesToRemove.insert(entry.m_edge);
                    return true;
                }
                return false;
            });
        }

        for (Edge_2* edge : edgesToRemove) {
            removeEdge(edge);
        }

        if (this->m_vertices[id]) {
            this->m_vertices[id]->markInactive();
        }
        m_freeIds.push(id);
    }

    std::vector<int> getNeighbors(int id) const override {
        std::vector<int> neighbors;
        if (id >= 0 && id < static_cast<int>(m_adjList.size())) {
            for (const auto& entry : m_adjList[id]) {
                if (entry.m_edge && entry.m_edge->isActive() &&
                    m_vertices[entry.m_to] && m_vertices[entry.m_to]->isActive()) {
                    neighbors.push_back(entry.m_to);
                }
            }
        }
        return neighbors;
    }

    Edge_2* getEdge(int fromId, int toId) const override {
        if (fromId >= 0 && fromId < static_cast<int>(m_adjList.size())) {
            for (const auto& entry : m_adjList[fromId]) {
                if (entry.m_to == toId && entry.m_edge && entry.m_edge->isActive()) {
                    return entry.m_edge;
                }
            }
        }
        return nullptr;
    }

    std::vector<Edge_2*> getEdgesFrom(int fromId) const override {
        std::vector<Edge_2*> result;
        if (fromId >= 0 && fromId < static_cast<int>(this->m_adjList.size())) {
            for (const auto& entry : this->m_adjList[fromId]) {
                if (entry.m_edge && entry.m_edge->isActive()) {
                    result.push_back(entry.m_edge);
                }
            }
        }
        return result;
    }
};