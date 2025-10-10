#pragma once

#include <string>
#include "Vertex.h"

/**
 * @class City
 * @brief Represents a city, inherits from Vertex
 */
class City : public Vertex {
private:
    /// @brief The population of the city (is later used to calculate the speed of a land vehicle in a densely populated city)
    long m_population;
public:
    /**
     * @brief Constructs a City object
     * @param name The name of the city
     * @param population The population of the city
     */
    City(const std::string& name, long population) : Vertex(name), m_population(population)  {}

    /**
     * @brief Gets the population of the city
     * @return The population
     */
    long getPopulation() const { return m_population; };

    /**
     * @brief Checks if the city is considered densely populated
     * @return True if the population is 1,000,000 or more, false otherwise
     */
    bool isDenselyPopulated() const {
        return m_population >= 1000000;
    }
};