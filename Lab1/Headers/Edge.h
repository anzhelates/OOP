#pragma once

#include <limits>
#include <string>
#include <vector>
#include "Vertex.h"
#include "Vehicle.h"

enum class RoadType { ROAD, RAIL, AIR, WATER };
enum class RoadCharacteristic { HIGHWAY, CITY, DIRT_ROAD, PARK, STANDARD };

struct Obstacle {
    std::string name;
    double delayHours = 0.0;
};

class Edge {
    private:
        Vertex* m_source;
        Vertex* m_destination;
        double m_distance;
        RoadType m_type;
        RoadCharacteristic m_characteristic;
        std::vector<Obstacle*> m_obstacles;
    public:
        Edge(Vertex* source, Vertex* destination, double distance,
            RoadType type = RoadType::ROAD, RoadCharacteristic characteristic = RoadCharacteristic::STANDARD)
            : m_source(source), m_destination(destination), m_distance(distance), m_type(type), m_characteristic(characteristic) {}
        virtual ~Edge() {
            for (auto* obstacle : m_obstacles) {
                delete obstacle;
            }
        }

        Vertex* getSource() const { return m_source; }
        Vertex* getDestination() const { return m_destination; }
        double getDistance() const { return m_distance; }
        RoadType getType() const { return m_type; }
        RoadCharacteristic getCharacteristic() const { return m_characteristic; }

        double calculateTravelTime(const Vehicle& vehicle) const {
            double travel = vehicle.travelTime(m_distance, m_type, m_characteristic);
            if (travel == std::numeric_limits<double>::infinity()) {
                return travel;
            }
            double totalDelay = 0.0;
            for (const auto* obstacle : m_obstacles) {
                totalDelay += obstacle->delayHours;
            }
            return travel + totalDelay;
        }

        void addObstacle(Obstacle* obstacle) {
            if(obstacle) m_obstacles.push_back(obstacle);
        }

        double getWeight(const Vehicle& vehicle) const {
            return calculateTravelTime(vehicle);
        }
};