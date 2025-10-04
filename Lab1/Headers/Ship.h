#pragma once

#include <string>
#include "Vehicle.h"

class Ship : public Vehicle {
public:
    Ship(const std::string& name = "Ship", double speed = 35, double fuel = 1800)
        : Vehicle(name, speed, fuel) {}
    bool canUse(RoadType type) const override { return type == RoadType::WATER; }
    double getSpeed(RoadCharacteristic characteristic) const override {
        return m_speed;
    }
};