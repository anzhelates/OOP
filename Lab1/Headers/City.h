#pragma once

#include <string>
#include "Vertex.h"

class City : public Vertex {
    private:
        long m_population = 0;
        double m_tolls = 0.0;
    public:
        explicit City(const std::string& name, long population = 0, double tolls = 0.0)
            : Vertex(name), m_population(population), m_tolls(tolls) {}
        ~City() override = default;
        double getToll() const override { return m_tolls; }
        long getPopulation() const { return m_population; }
};