#pragma once

#include <string>
#include "EnumClasses.h"

class City;

/**
 * @class Vehicle
 * @brief An abstract base class for all types of vehicles
 * @details Common interface for vehicles, including calculating speed under various conditions and fuel consumption
 */
class Vehicle {
    protected:
        std::string m_name;
        double m_speed;
        double m_fuelEfficiency;

    public:
        /**
         * @brief Constructs a Vehicle
         * @param name The name of the vehicle
         * @param speed The speed in km/h
         * @param fuelEfficiency The fuel consumption in liters per 100 km
         */
        Vehicle(const std::string& name, double speed, double fuelEfficiency)
            : m_name(name), m_speed(speed), m_fuelEfficiency(fuelEfficiency) {}

        virtual ~Vehicle() = default;

        /** @brief m_name getter */
        const std::string& getName() const { return m_name; }

        /**
         * @brief Pure virtual function to calculate the vehicle's effective speed
         * @param cause The cause of an obstacle affecting speed
         * @param characteristic The characteristic of the path
         * @return The calculated speed in km/h
         */
        virtual double getSpeed(ObstacleCause cause, RoadCharacteristic characteristic) const = 0;

        /**
         * @brief Checks whether the vehicle can use a certain RoadType
         * @param type The type of the path
         */
        virtual bool canUse(RoadType type) const = 0;

        /**
         * @brief Calculates the fuel needed to travel a given distance
         * @param distance The distance to travel in kilometers
         */
        virtual double calculateFuel(double distance) const {
            return (distance / 100.0) * m_fuelEfficiency;
        }
};