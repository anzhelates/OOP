#pragma once

#include "Vehicle.h"

class Plane : public Vehicle {
public:
    Plane(const string& name = "Plane", double speed = 850, double fuel = 4000)
        : Vehicle(name, speed, fuel) {}
    double getSpeed(RoadType type) const override {
        return (type == RoadType::AIR) ? m_speed : 0;
    }
};