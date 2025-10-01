#pragma once

#include "Vehicle.h"

class Car : public Vehicle {
    public:
        Car(const string& name = "Car", double speed = 80, double fuel = 7)
            : Vehicle(name, speed, fuel) {}
        double getSpeed(RoadType type) const override {
            return (type == RoadType::ROAD) ? m_speed : 0;
        }
};