#pragma once

#include "Vehicle.h"

class Train : public Vehicle {
public:
    Train(const string& name = "Trainn", double speed = 160, double fuel = 25)
        : Vehicle(name, speed, fuel) {}
    double getSpeed(RoadType type) const override {
        return (type == RoadType::RAIL) ? m_speed : 0;
    }
};