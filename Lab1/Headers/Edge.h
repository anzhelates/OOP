#pragma once

#include <string>
#include "Vertex.h"
#include "Vehicle.h"
using namespace std;

struct Obstacle {
    string name;
    double delayHours;
};

class Edge {
    private:
        Vertex* m_source;
        Vertex* m_destination;
        double m_distance;
        RoadType m_type;
        vector<Obstacle*> m_obstacles;
    public:
        Edge(Vertex* source, Vertex* destination, double distance, RoadType type)
            : m_source(source), m_destination(destination), m_distance(distance), m_type(type) {}
        virtual ~Edge() {
            for (auto* obstacle : m_obstacles) {
                delete obstacle;
            }
        }

    Vertex* getSource() const { return m_source; }
    Vertex* getDestination() const { return m_destination; }
    double getDistance() const { return m_distance; }
    RoadType getType() const { return m_type; }

    void addObstacle(Obstacle* obstacle) { m_obstacles.push_back(obstacle); }
};