#pragma once

#include <string>

class Graph_2;
class AdjacencyList_2;

class Vertex_2 {
private:
    int m_id = -1;
    std::string m_name;
    bool m_active = true;

public:
    Vertex_2() : m_name() {}
    explicit Vertex_2(const std::string& name) : m_name(name) {}
    virtual ~Vertex_2() = default;

    void setId(int id) { m_id = id; }
    int getId() const { return m_id; }
    const std::string& getName() const { return m_name; }
    void setName(const std::string& name) { m_name = name; }

    void markInactive() { m_active = false; }
    void markActive() { m_active = true; }
    bool isActive() const { return m_active; }

    friend class Graph_2;
    friend class AdjacencyList_2;
};