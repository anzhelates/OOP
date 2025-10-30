#pragma once

#include <vector>

struct EdgeId {
    int from = -1;
    int to = -1;
};

struct BFSState {
    int currentVertex = -1;
    std::vector<EdgeId> currentEdges;
    std::vector<int> visitedVertices;
    std::vector<EdgeId> visitedEdges;
};