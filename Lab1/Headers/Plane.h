#pragma once

#include <string>
#include "Vehicle.h"

class Plane : public Vehicle {
public:
    Plane(const std::string& name = "Plane", double speed = 850, double fuel = 4000)
        : Vehicle(name, speed, fuel) {}
    bool canUse(RoadType type) const override { return type == RoadType::AIR; }
    double getSpeed(RoadCharacteristic characteristic) const override {
        return m_speed;
    }
};