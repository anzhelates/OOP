#pragma once

#include <vector>
#include "Edge.h"
#include "Vehicle.h"
#include "Vertex.h"

/**
 * @class Route
 * @brief Represents a calculated path from a start vertex to an end vertex
 * @details Provides methods to calculate total time, distance, and fuel consumption
 */
class Route {
    private:
        Vertex* m_startVertex = nullptr; ///< The starting vertex of the route
        Vertex* m_endVertex = nullptr; ///< The ending vertex of the route
        const Vehicle* m_vehicle = nullptr; ///< The vehicle used for the route
        std::vector<Edge*> m_path; ///< The sequence of edges that form the path

    public:
        Route() = default;

        /**
         * @brief Constructs a Route with start, end, and vehicle
         * @param start The starting vertex
         * @param end The ending vertex
         * @param vehicle The vehicle to be used
         */
        Route(Vertex* start, Vertex* end, const Vehicle* vehicle)
            : m_startVertex(start), m_endVertex(end), m_vehicle(vehicle) {}

        /** @brief Setters and getters */
        void setVehicle(const Vehicle* vehicle) { m_vehicle = vehicle; }
        void setStart(Vertex* start) { m_startVertex = start; }
        void setEnd(Vertex* end) { m_endVertex = end; }

        Vertex* getStart() const { return m_startVertex; }
        Vertex* getEnd() const { return m_endVertex; }
        const Vehicle* getVehicle() const { return m_vehicle; }

        /** @brief Adds an edge to the end of the path */
        void addEdge(Edge* edge) { if (edge) m_path.push_back(edge); }
        /** @brief Gets the complete path */
        const std::vector<Edge*>& getPath() const { return m_path; }

        /**
         * @brief Calculates the total travel time for the route
         * @param vehicle The vehicle used for travel
         */
        double totalTime(const Vehicle& vehicle) const {
           double total = 0.0;
             for (auto* edge : m_path) {
                 total += edge->calculateTravelTime(vehicle);
             }
             return total;
        }

        /** @brief Calculates the total distance of the route */
        double totalDistance() const {
            double total = 0.0;
            for (auto* edge : m_path) {
                total += edge->getDistance();
            }
            return total;
        }

        /** @brief Calculates the total fuel consumption for the route. */
        double totalFuel(const Vehicle& vehicle) const {
             double total = 0.0;
             for (auto* edge : m_path) {
                 double distance = edge->getDistance();
                 total += vehicle.calculateFuel(distance);
             }
             return total;
        }
};