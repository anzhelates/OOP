#pragma once

#include <string>
#include "Vehicle.h"

class Car : public Vehicle {
    public:
        Car(const std::string& name = "Car", double speed = 80, double fuel = 7)
            : Vehicle(name, speed, fuel) {}

        bool canUse(RoadType type) const override { return type == RoadType::ROAD; }
        double getSpeed(RoadCharacteristic characteristic) const override {
            double speed = m_speed;
            switch (characteristic) {
                case RoadCharacteristic::HIGHWAY:
                    speed *= 1.2;
                    break;
                case RoadCharacteristic::CITY:
                    speed *= 0.7;
                    break;
                case RoadCharacteristic::DIRT_ROAD:
                    speed *= 0.4;
                    break;
                case RoadCharacteristic::PARK:
                    speed *= 0.5;
                    break;
                default: break;
            }
            return speed;
        }
};