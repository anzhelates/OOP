#pragma once

#include <string>
#include "Vehicle.h"

class Train : public Vehicle {
public:
    Train(const std::string& name = "Train", double speed = 160, double fuel = 25)
        : Vehicle(name, speed, fuel) {}
    bool canUse(RoadType type) const override { return type == RoadType::RAIL; }
    double getSpeed(RoadCharacteristic characteristic) const override {
        return m_speed;
    }
};