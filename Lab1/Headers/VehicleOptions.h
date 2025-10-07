#pragma once

#include <string>
#include "Vehicle.h"

class VehicleOptions : public Vehicle {
    public:
        VehicleOptions(const std::string& name = "Car", double speed = 80, double fuel = 7)
            : Vehicle(name, speed, fuel) {}

        bool canUse(RoadType type) const override { return type == RoadType::ROAD; }
        double getSpeed(RoadCharacteristic characteristic) const override {
            double speed = m_speed;
            switch (characteristic) {
                case RoadCharacteristic::HIGHWAY:
                    speed *= 1.2;
                    break;
                case RoadCharacteristic::CITY_STREET:
                    speed *= 0.7;
                    break;
                case RoadCharacteristic::DIRT_ROAD:
                    speed *= 0.4;
                    break;
                case RoadCharacteristic::PARK_ROAD:
                    speed *= 0.5;
                    break;
                default: break;
            }
            return speed;
        }
};

class Train : public Vehicle {
public:
    Train(const std::string& name = "Train", double speed = 160, double fuel = 25)
        : Vehicle(name, speed, fuel) {}
    bool canUse(RoadType type) const override { return type == RoadType::RAIL; }
    double getSpeed(RoadCharacteristic characteristic) const override {
        return m_speed;
    }
};

class Plane : public Vehicle {
public:
    Plane(const std::string& name = "Plane", double speed = 850, double fuel = 4000)
        : Vehicle(name, speed, fuel) {}
    bool canUse(RoadType type) const override { return type == RoadType::AIR; }
    double getSpeed(RoadCharacteristic characteristic) const override {
        return m_speed;
    }
};

class Ship : public Vehicle {
public:
    Ship(const std::string& name = "Ship", double speed = 35, double fuel = 1800)
        : Vehicle(name, speed, fuel) {}
    bool canUse(RoadType type) const override { return type == RoadType::WATER; }
    double getSpeed(RoadCharacteristic characteristic) const override {
        return m_speed;
    }
};