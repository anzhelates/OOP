#pragma once

#include <string>
#include "Vertex.h"

class City : public Vertex {
    private:
        long m_population;
    public:
        explicit City(const std::string& name, long population)
            : Vertex(name), m_population(population) {}
        ~City() override = default;
        long getPopulation() const { return m_population; }
};