#include "Edge.h"
#include <limits>

ObstacleImpact Edge::getImpact(RoadType type) {
    switch (type) {
        case RoadType::ROAD: return ObstacleImpact::ROAD_ONLY;
        case RoadType::RAIL: return ObstacleImpact::RAIL_ONLY;
        case RoadType::AIR: return ObstacleImpact::AIR_ONLY;
        case RoadType::WATER: return ObstacleImpact::WATER_ONLY;
    }
    return ObstacleImpact::ALL_VEHICLES;
}

Edge::Edge(Vertex* source, Vertex* destination, double distance,
           RoadType type, RoadCharacteristic characteristic)
    : m_source(source), m_destination(destination), m_distance(distance), m_type(type), m_characteristic(characteristic) {}

Edge::~Edge() {
    for (auto* obstacle : m_obstacles) {
        delete obstacle;
    }
    m_obstacles.clear();
}

void Edge::addObstacle(Obstacle* obstacle) {
    if (obstacle) m_obstacles.push_back(obstacle);
}

void Edge::applyImpact(double& delay, double& speedFactor) const {
    delay = 0.0;
    speedFactor = 1.0;
    for (auto* obstacle : m_obstacles) {
        if (!obstacle) continue;
        bool affected = (obstacle->impact == ObstacleImpact::ALL_VEHICLES)
            || (obstacle->impact == getImpact(m_type));
        if (affected) {
            delay += obstacle->delayHours;
            speedFactor *= obstacle->speedMultiplier;
        }
    }
}
double Edge::calculateTravelTime(const Vehicle& vehicle) const {
    if (!vehicle.canUse(m_type)) {
        return std::numeric_limits<double>::infinity();
    }
    double delay = 0.0;
    double speedFactor = 1.0;
    applyImpact(delay, speedFactor);
    double basicTime = vehicle.travelTime(m_distance, m_type, m_characteristic);
    if (basicTime == std::numeric_limits<double>::infinity()) {
        return basicTime;
    }
    if (speedFactor <= 0) {
        return std::numeric_limits<double>::infinity();
    }
    return basicTime / speedFactor + delay;
}