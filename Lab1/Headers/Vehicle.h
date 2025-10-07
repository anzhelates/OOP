#pragma once

#include <string>
#include <limits>
#include "EnumClasses.h"

class Vehicle {
    protected:
        std::string m_name;
        double m_speed;
        double m_fuelEfficiency;

    public:
        Vehicle(const std::string& name, double speed, double fuelEfficiency)
            : m_name(name), m_speed(speed), m_fuelEfficiency(fuelEfficiency) {}
        virtual ~Vehicle() = default;

        const std::string& getName() const { return m_name; }
        virtual double getSpeed(RoadCharacteristic characteristic) const = 0;
        virtual bool canUse(RoadType type) const = 0;
        virtual double calculateFuel(double distance) const {
            return (distance / 100.0) * m_fuelEfficiency;
        }
        virtual double travelTime(double distance, RoadType type, RoadCharacteristic characteristic) const {
            if (!canUse(type)) {
                return std::numeric_limits<double>::infinity();
            }
            double current_speed = getSpeed(characteristic);
            if (current_speed <= 0.0) {
                return std::numeric_limits<double>::infinity();
            }
            return distance / current_speed;
        }
};