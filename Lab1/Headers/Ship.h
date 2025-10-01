#pragma once

#include "Vehicle.h"

class Ship : public Vehicle {
public:
    Ship(const string& name = "Ship", double speed = 40, double fuel = 1800)
        : Vehicle(name, speed, fuel) {}
    double getSpeed(RoadType type) const override {
        return (type == RoadType::WATER) ? m_speed : 0;
    }
};