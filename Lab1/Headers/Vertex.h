#pragma once

#include <string>
#include <vector>
#include "Vehicle.h"

using namespace std;

class Vertex {
    private:
        string m_name;
        vector<Vehicle*> m_availableVehicles;
    public:
        Vertex() : m_name() {}
        explicit Vertex(const string& name) : m_name(name) {}
        virtual ~Vertex() = default;

        void setName(const string& name) { m_name = name; }
        string getName() const { return m_name; }

        void addVehicle(Vehicle* vehicle) { m_availableVehicles.push_back(vehicle); }
        vector<Vehicle*> getAvailableVehicles() const { return m_availableVehicles; }
};