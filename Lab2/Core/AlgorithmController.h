#pragma once

#include "Graph_2.h"
#include "AlgorithmStep.h"
#include <vector>
#include <queue>

enum class AlgorithmType { BFS, DFS, Dijkstra };

class AlgorithmController {
private:
    Graph_2* m_graph = nullptr;
    AlgorithmType m_algorithm;

    std::vector<BFSState> m_history;
    size_t m_currentStep = 0;

    std::queue<int> m_bfs_q;
    std::vector<bool> m_bfs_visited;
    std::vector<int> m_allVisitedVertices;
    std::vector<EdgeId> m_allVisitedEdges;

public:
    explicit AlgorithmController(Graph_2* graph = nullptr)
        : m_graph(graph), m_algorithm(AlgorithmType::BFS) {}

    void setGraph(Graph_2* graph) {
        m_graph = graph;
        reset();
    }

    void setAlgorithm(AlgorithmType alg) {
        m_algorithm = alg;
        reset();
    }

    AlgorithmType getAlgorithm() const {
        return m_algorithm;
    }

    bool start(int startVertexId, BFSState& state) {
        reset();
        if (!m_graph || m_graph->getNumberOfVertices() == 0) return false;
        if (!m_graph->getVertexById(startVertexId)) {
            bool found = false;
            for(const auto& v : m_graph->getVertices()) {
                if(v && v->isActive()) {
                    startVertexId = v->getId();
                    found = true;
                    break;
                }
            }
            if(!found) return false;
        }

        if (m_algorithm == AlgorithmType::BFS) {
            m_bfs_q = std::queue<int>();
            m_bfs_visited.assign(m_graph->getNumberOfVertices(), false);
            m_allVisitedVertices.clear();
            m_allVisitedEdges.clear();

            m_bfs_q.push(startVertexId);
            m_bfs_visited[startVertexId] = true;
            m_allVisitedVertices.push_back(startVertexId);

            BFSState initialState;
            initialState.visitedVertices = m_allVisitedVertices;
            initialState.currentVertex = -1;
            m_history.push_back(initialState);

            state = initialState;
            return true;
        }
        return false;
    }

    bool nextStep(BFSState& state) {
        if (!m_graph) return false;
        if (m_currentStep + 1 < m_history.size()) {
            m_currentStep++;
            state = m_history[m_currentStep];
            return true;
        }
        if (m_algorithm == AlgorithmType::BFS) {
            if (m_bfs_q.empty()) return false;

            BFSState newState = m_history.back();
            newState.currentEdges.clear();

            int current = m_bfs_q.front();
            m_bfs_q.pop();

            newState.currentVertex = current;
            std::vector<int> neighbors = m_graph->getNeighbors(current);
            for (int neighbor : neighbors) {
                if (!m_bfs_visited[neighbor]) {
                    m_bfs_visited[neighbor] = true;
                    m_bfs_q.push(neighbor);

                    Edge_2* edge = m_graph->getEdge(current, neighbor);
                    if (!edge && !m_graph->isDirected()) {
                        edge = m_graph->getEdge(neighbor, current);
                    }

                    if (edge) {
                        EdgeId id = {edge->getFrom(), edge->getTo()};
                        newState.currentEdges.push_back(id);
                        m_allVisitedEdges.push_back(id);
                    }
                    m_allVisitedVertices.push_back(neighbor);
                }
            }

            newState.visitedVertices = m_allVisitedVertices;
            newState.visitedEdges = m_allVisitedEdges;

            m_history.push_back(newState);
            m_currentStep++;
            state = newState;
            return true;
        }
        return false;
    }

    bool prevStep(BFSState& state) {
        if (m_currentStep == 0) return false;
        m_currentStep--;
        state = m_history[m_currentStep];
        return true;
    }

    void reset() {
        m_currentStep = 0;
        m_history.clear();
        m_bfs_q = std::queue<int>();
        m_bfs_visited.clear();
        m_allVisitedVertices.clear();
        m_allVisitedEdges.clear();
    }

    size_t getCurrentStep() const { return m_currentStep; }
};