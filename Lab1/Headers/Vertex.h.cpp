#pragma once

#include <string>
using namespace std;

class Vertex {
    private:
        string m_name;
    public:
        Vertex() : m_name() {}
        explicit Vertex(const string& name) : m_name(name) {}
        virtual ~Vertex() = default;

        void setName(const string& name) { m_name = name; }
        string getName() const { return m_name; }
};