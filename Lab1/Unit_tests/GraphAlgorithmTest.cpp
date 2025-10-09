#include "doctest.h"

#include "GraphAlgorithm.h"
#include "AdjacencyList.h"
#include "AdjacencyMatrix.h"
#include "City.h"
#include "Edge.h"
#include "EnumClasses.h"
#include "Car.h"
#include "LandVehicle.h"
#include <algorithm>

template<typename GraphType>
GraphType buildSimpleGraph(bool directed = false) {
    GraphType g(directed);

    City* a = new City("A", 1000);
    City* b = new City("B", 2000);
    City* c = new City("C", 1500);

    int idA = g.addVertex(a);
    int idB = g.addVertex(b);
    int idC = g.addVertex(c);

    g.addEdge(new Edge(a, b, 10.0, RoadType::ROAD));
    g.addEdge(new Edge(b, c, 20.0, RoadType::ROAD));
    g.addEdge(new Edge(a, c, 50.0, RoadType::ROAD));

    return g;
}

TEST_CASE("BFS: empty graph") {
    AdjacencyList<City, Edge> g;
    auto res = GraphAlgorithms::BFS(g, 0);
    CHECK(res.empty());
}

TEST_CASE("BFS: single vertex") {
    AdjacencyList<City, Edge> g;
    City* a = new City("A", 1000);
    int idA = g.addVertex(a);

    auto res = GraphAlgorithms::BFS(g, idA);
    REQUIRE(res.size() == 1);
    CHECK(res[0] == idA);
}

TEST_CASE("BFS: simple undirected graph") {
    auto g = buildSimpleGraph<AdjacencyList<City, Edge>>(false);
    auto res = GraphAlgorithms::BFS(g, 0);

    CHECK(res.size() == 3);
    CHECK(std::find(res.begin(), res.end(), 0) != res.end());
    CHECK(std::find(res.begin(), res.end(), 1) != res.end());
    CHECK(std::find(res.begin(), res.end(), 2) != res.end());
}

TEST_CASE("DFS: empty graph") {
    AdjacencyMatrix<City, Edge> g;
    auto res = GraphAlgorithms::DFS(g, 0);
    CHECK(res.empty());
}

TEST_CASE("DFS: simple graph") {
    auto g = buildSimpleGraph<AdjacencyMatrix<City, Edge>>(false);
    auto res = GraphAlgorithms::DFS(g, 0);

    CHECK(res.size() == 3);
    CHECK(std::find(res.begin(), res.end(), 0) != res.end());
    CHECK(std::find(res.begin(), res.end(), 1) != res.end());
    CHECK(std::find(res.begin(), res.end(), 2) != res.end());
}

TEST_CASE("Dijkstra: unreachable vertices") {
    AdjacencyList<City, Edge> g;
    City* a = new City("A", 1000);
    City* b = new City("B", 2000);
    int idA = g.addVertex(a);
    int idB = g.addVertex(b);

    Car car("Car", 100, 8);
    auto dist = GraphAlgorithms::Dijkstra(g, idA, car);

    REQUIRE(dist.size() == 2);
    CHECK(dist[idA] == doctest::Approx(0.0));
    CHECK(dist[idB] == std::numeric_limits<double>::infinity());
}

TEST_CASE("Dijkstra: shortest path") {
    auto g = buildSimpleGraph<AdjacencyList<City, Edge>>(false);
    Car car("Car", 100, 8);

    auto dist = GraphAlgorithms::Dijkstra(g, 0, car);
    REQUIRE(dist.size() == 3);

    CHECK(dist[0] == doctest::Approx(0.0));
    CHECK(dist[1] < dist[2]);
}

TEST_CASE("Dijkstra: obstacle impact") {
    AdjacencyList<City, Edge> g;
    City* a = new City("A", 1000);
    City* b = new City("B", 2000);
    int idA = g.addVertex(a);
    int idB = g.addVertex(b);

    Edge* ab = new Edge(a, b, 100.0, RoadType::ROAD);
    ab->addObstacle(new Obstacle{"Traffic jam", 2.0, ObstacleCause::TRAFFIC_JAM});
    g.addEdge(ab);

    Car car("Car", 100, 8);
    auto dist = GraphAlgorithms::Dijkstra(g, idA, car);

    REQUIRE(dist.size() == 2);
    CHECK(dist[idB] > 1.0);
}

TEST_CASE("Dijkstra: directed graph") {
    AdjacencyList<City, Edge> g(true);
    City* a = new City("A", 1000);
    City* b = new City("B", 2000);

    int idA = g.addVertex(a);
    int idB = g.addVertex(b);

    g.addEdge(new Edge(b, a, 10.0, RoadType::ROAD));

    Car car("Car", 100, 8);
    auto dist = GraphAlgorithms::Dijkstra(g, idA, car);
    CHECK(dist[idB] == std::numeric_limits<double>::infinity());
}

TEST_CASE("Dijkstra: multiple obstacles on one edge") {
    AdjacencyList<City, Edge> g;
    City* a = new City("A", 1000);
    City* b = new City("B", 2000);
    int idA = g.addVertex(a);
    int idB = g.addVertex(b);

    Edge* ab = new Edge(a, b, 100.0, RoadType::ROAD);
    ab->addObstacle(new Obstacle{"Jam", 2.0, ObstacleCause::TRAFFIC_JAM});
    ab->addObstacle(new Obstacle{"Construction", 1.0, ObstacleCause::CONSTRUCTION});
    g.addEdge(ab);

    Car car("Car", 100, 8);
    auto dist = GraphAlgorithms::Dijkstra(g, idA, car);
    CHECK(dist[idB] > 1.0);
}

TEST_CASE("Graph: remove vertex and edge") {
    AdjacencyList<City, Edge> g(false);
    City* a = new City("A", 1000);
    City* b = new City("B", 2000);
    int idA = g.addVertex(a);
    int idB = g.addVertex(b);

    Edge* e = new Edge(a, b, 10.0, RoadType::ROAD);
    g.addEdge(e);

    REQUIRE_FALSE(g.getNeighbors(idA).empty());
    g.removeEdge(e);
    CHECK(g.getNeighbors(idA).empty());

    g.removeVertex(idB);
    auto vB = g.getVertexById(idB);
    CHECK(vB != nullptr);
    CHECK(vB->isActive() == false);
}

TEST_CASE("Dijkstra: (f.e.) bike is slower than car") {
    AdjacencyList<City, Edge> g(false);
    City* a = new City("A", 1000);
    City* b = new City("B", 2000);

    int idA = g.addVertex(a);
    int idB = g.addVertex(b);

    g.addEdge(new Edge(a, b, 100.0, RoadType::ROAD));

    Car car("Car", 100, 8);
    LandVehicle bike("Bike", 20, 0);

    auto distBike = GraphAlgorithms::Dijkstra(g, idA, bike);
    auto distCar = GraphAlgorithms::Dijkstra(g, idA, car);

    CHECK(distBike[idB] > distCar[idB]);
}

TEST_CASE("isConnected: undirected connected") {
    auto g = buildSimpleGraph<AdjacencyList<City, Edge>>(false);
    CHECK(GraphAlgorithms::isConnected(g));
}

TEST_CASE("isConnected: undirected disconnected") {
    AdjacencyList<City, Edge> g(false);
    City* a = new City("A", 1000);
    City* b = new City("B", 2000);
    City* c = new City("C", 1500);

    g.addVertex(a);
    g.addVertex(b);
    g.addVertex(c);

    g.addEdge(new Edge(a, b, 10.0, RoadType::ROAD));
    CHECK_FALSE(GraphAlgorithms::isConnected(g));
}

TEST_CASE("isConnected: directed connected") {
    AdjacencyList<City, Edge> g(true);
    City* a = new City("A", 1000);
    City* b = new City("B", 2000);
    City* c = new City("C", 1500);

    g.addVertex(a);
    g.addVertex(b);
    g.addVertex(c);

    g.addEdge(new Edge(a, b, 10.0, RoadType::ROAD));
    g.addEdge(new Edge(b, c, 5.0, RoadType::ROAD));

    CHECK(GraphAlgorithms::isConnected(g));
}

TEST_CASE("isConnected: directed disconnected") {
    AdjacencyList<City, Edge> g(true);
    City* a = new City("A", 1000);
    City* b = new City("B", 2000);
    City* c = new City("C", 1500);

    g.addVertex(a);
    g.addVertex(b);
    g.addVertex(c);

    g.addEdge(new Edge(a, b, 10.0, RoadType::ROAD));
    CHECK_FALSE(GraphAlgorithms::isConnected(g));
}