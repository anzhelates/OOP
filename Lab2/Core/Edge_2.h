#pragma once

#include "Vertex_2.h"

class Edge_2 {
private:
    Vertex_2* m_source;
    Vertex_2* m_destination;
    double m_weight;
    bool m_active = false;

public:
    Edge_2(Vertex_2* source, Vertex_2* destination, double weight = 1.0)
    : m_source(source), m_destination(destination), m_weight(weight), m_active(true) {}
    virtual ~Edge_2() = default;

    Vertex_2* getSource() const { return m_source; }
    Vertex_2* getDestination() const { return m_destination; }
    double getWeight() const { return m_weight; }
    void setWeight(double weight) { m_weight = weight; }
    int getFrom() const { return m_source ? m_source->getId() : -1; }
    int getTo() const { return m_destination ? m_destination->getId() : -1; }

    void markInactive() { m_active = false; }
    void markActive() { m_active = true; }
    bool isActive() const { return m_active; }
};