#include "Edge.h"
#include <limits>

/**
 * @brief Constructs an Edge object
 * @param source Pointer to the source vertex
 * @param destination Pointer to the destination vertex
 * @param distance The length of the edge in km
 * @param type The type of road (ROAD, RAIL, AIR, WATER).
 * @param characteristic The characteristic of the road (HIGHWAY, CITY_STREET, DIRT_ROAD, PARK_ROAD, STANDARD, DENSELY_POPULATED_CITY, OTHER)
 */
Edge::Edge(Vertex* source, Vertex* destination, double distance,
           RoadType type, RoadCharacteristic characteristic)
    : m_source(source), m_destination(destination), m_distance(distance), m_type(type), m_characteristic(characteristic), m_active(true) {}

/**
 * @brief Destructor for the Edge class
 * @details Responsible for deleting Obstacle objects
 */
Edge::~Edge() {
    for (auto* obstacle : m_obstacles) {
        delete obstacle;
    }
    m_obstacles.clear();
}

/**
 * @brief Adds an obstacle to the edge
 * @param obstacle Pointer to the obstacle to add
 */
void Edge::addObstacle(Obstacle* obstacle) {
    if (obstacle) m_obstacles.push_back(obstacle);
}

/**
 * @brief Calculates the total time of travel on a given vehicle
 * @param vehicle The vehicle that will be used
 * @return The total travel time in hours, if the vehicle cannot be used - infinity
 */
double Edge::calculateTravelTime(const Vehicle& vehicle) const {
    if (!m_active || !vehicle.canUse(m_type)) {
        return std::numeric_limits<double>::infinity();
    }

    double delay = 0.0;
    double final_speed = vehicle.getSpeed(ObstacleCause::CUSTOM_DELAY, m_characteristic);

    for (const auto* obstacle : m_obstacles) {
        if (!obstacle) continue;

        delay += obstacle->delayHours;
        double speed_with_obstacle = vehicle.getSpeed(obstacle->cause, m_characteristic);

        final_speed = std::min(final_speed, speed_with_obstacle);
    }
    return m_distance / final_speed + delay;
}