#pragma once

#include <string>
#include <vector>
#include "Vehicle.h"

template <typename TVertex, typename TEdge> class Graph;

class Vertex {
    private:
        int m_id = -1;
        std::string m_name;
        std::vector<Vehicle*> m_availableVehicles;
    protected:
        void setId(int id) {
            m_id = id;
        }
    public:
        Vertex() : m_name("untitled") {}
        explicit Vertex(const std::string& name) : m_name(name) {}
        virtual ~Vertex() = default;

        int getId() const { return m_id; }
        std::string getName() const { return m_name; }
        virtual double getToll() const { return 0.0; }

        void addVehicle(Vehicle* vehicle) { m_availableVehicles.push_back(vehicle); }
        const std::vector<Vehicle*>& getAvailableVehicles() const { return m_availableVehicles; }

        template <typename TVertex, typename TEdge> friend class Graph;
};