#pragma once

#include <string>
using namespace std;

enum class RoadType { ROAD, RAIL, AIR, WATER };

class Vehicle {
    protected:
        string m_name;
        double m_speed;
        double m_fuelEfficiency;
    public:
        Vehicle(const string& name, double maxSpeed, double fuelEfficiency)
            : m_name(name), m_speed(speed), m_fuelEfficiency(fuelEfficiency)  {}
        virtual ~Vehicle() = default;

        string getName() const { return m_name; }
        virtual double getSpeed() const = 0;
        virtual double calculateFuel(double distance) const {
            return (distance / 100.0) * m_fuelEfficiency;
        }
        virtual bool canUse(RoadType type) const = 0;
        virtual double getObstacle() const = 0;
        virtual double travelTime(double distance) const {
            return distance / m_speed;
        }
};