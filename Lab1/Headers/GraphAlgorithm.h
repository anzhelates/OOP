#pragma once

#include <vector>
#include <queue>
#include <stack>
#include <limits>
#include "Graph.h"
#include "Vehicle.h"

class GraphAlgorithms {
public:
    template <typename TVertex, typename TEdge>
static std::vector<int> BFS(const Graph<TVertex, TEdge>& graph, int startId) {
        std::vector<int> order;
        int n = graph.getNumberOfVertices();
        if (startId < 0 || startId >= n) return order;

        std::vector<bool> visited(n, false);
        std::queue<int> q;

        visited[startId] = true;
        q.push(startId);

        while (!q.empty()) {
            int to = q.front(); q.pop();
            order.push_back(to);

            std::vector<int> neighbors = graph.getNeighbors(to);
            for (int from : neighbors) {
                if (from >= 0 && from < n && !visited[from]) {
                    visited[from] = true;
                    q.push(from);
                }
            }
        }
        return order;
    }

    template <typename TVertex, typename TEdge>
static std::vector<int> DFS(const Graph<TVertex, TEdge>& graph, int startId) {
        std::vector<int> order;
        int n = graph.getNumberOfVertices();
        if (startId < 0 || startId >= n) return order;

        std::vector<bool> visited(n, false);
        std::stack<int> st;
        st.push(startId);

        while (!st.empty()) {
            int to = st.top(); st.pop();
            if (!visited[to]) {
                visited[to] = true;
                order.push_back(to);
                std::vector<int> neighbors = graph.getNeighbors(to);
                for (int i = static_cast<int>(neighbors.size()) - 1; i >= 0; --i) {
                    int from = neighbors[i];
                    if (!visited[from]) st.push(from);
                }
            }
        }
        return order;
    }

    template <typename TVertex, typename TEdge>
static std::vector<double> Dijkstra(const Graph<TVertex, TEdge>& graph, int startId, const Vehicle& vehicle) {
        int n = graph.getNumberOfVertices();
        std::vector<double> dist(n, std::numeric_limits<double>::infinity());

        if (startId < 0 || startId >= n) {
            return dist;
        }

        dist[startId] = 0.0;

        using PDI = std::pair<double, int>;
        std::priority_queue<PDI, std::vector<PDI>, std::greater<PDI>> pq;

        pq.push({0.0, startId});

        while (!pq.empty()) {
            double currentDist = pq.top().first;
            int currentVertex = pq.top().second;
            pq.pop();

            if (currentDist > dist[currentVertex]) {
                continue;
            }

            std::vector<TEdge*> outgoingEdges = graph.getEdgesFrom(currentVertex);

            for (TEdge* edge : outgoingEdges) {
                if (!edge || !edge->isActive()) {
                    continue;
                }

                int neighborVertex = edge->getDestination()->getId();
                double travelTime = edge->calculateTravelTime(vehicle);

                if (travelTime == std::numeric_limits<double>::infinity()) {
                    continue;
                }

                double newDist = dist[currentVertex] + travelTime;

                if (newDist < dist[neighborVertex]) {
                    dist[neighborVertex] = newDist;
                    pq.push({newDist, neighborVertex});
                }
            }
        }
        return dist;
    }
    template <typename TVertex, typename TEdge>
    static bool isConnected(const Graph<TVertex, TEdge>& graph) {
        int n = graph.getNumberOfVertices();
        if (n < 2) return true;

        std::vector<bool> visited(n, false);
        std::stack<int> st;
        st.push(0);
        visited[0] = true;
        int count = 1;

        while (!st.empty()) {
            int from = st.top(); st.pop();

            // сусіди по напрямку
            for (int to : graph.getNeighbors(from)) {
                if (!visited[to]) {
                    visited[to] = true;
                    st.push(to);
                    count++;
                }
            }

            // якщо граф орієнтований — додаємо ще "зворотних сусідів"
            if (graph.isDirected()) {
                for (auto* edge : graph.getEdges()) {
                    if (edge->getTo() == from) {
                        int pred = edge->getFrom();
                        if (!visited[pred]) {
                            visited[pred] = true;
                            st.push(pred);
                            count++;
                        }
                    }
                }
            }
        }
        return count == n;
    }
};