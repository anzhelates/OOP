#include "doctest.h"

#include "Edge.h"
#include "LandVehicle.h"
#include "RailVehicle.h"
#include "AirVehicle.h"
#include "WaterVehicle.h"
#include "Car.h"
#include "Underground.h"
#include "Route.h"
#include "City.h"
#include <limits>

TEST_CASE("Vehicle: speed with different road characteristics") {
    Car car("Car", 100.0, 8.0);

    CHECK(car.getSpeed(ObstacleCause::CUSTOM_DELAY, RoadCharacteristic::STANDARD) == doctest::Approx(100.0));
    CHECK(car.getSpeed(ObstacleCause::CUSTOM_DELAY, RoadCharacteristic::HIGHWAY) == doctest::Approx(120.0));
    CHECK(car.getSpeed(ObstacleCause::CUSTOM_DELAY, RoadCharacteristic::CITY_STREET) == doctest::Approx(80.0));
    CHECK(car.getSpeed(ObstacleCause::CUSTOM_DELAY, RoadCharacteristic::DIRT_ROAD) == doctest::Approx(60.0));
    CHECK(car.getSpeed(ObstacleCause::CUSTOM_DELAY, RoadCharacteristic::DENSELY_POPULATED_CITY) == doctest::Approx(70.0));
}

TEST_CASE("Vehicle: canUse for different types") {
    Car car("Car", 100, 8);
    Underground metro("Metro", 80, 5);
    AirVehicle plane("Plane", 900, 3000);
    WaterVehicle ship("Ship", 40, 1500);

    CHECK(car.canUse(RoadType::ROAD) == true);
    CHECK(car.canUse(RoadType::RAIL) == false);
    CHECK(metro.canUse(RoadType::RAIL) == true);
    CHECK(metro.canUse(RoadType::ROAD) == false);
    CHECK(plane.canUse(RoadType::AIR) == true);
    CHECK(ship.canUse(RoadType::WATER) == true);
}

TEST_CASE("Edge: calculateTravelTime") {
    City* c1 = new City("A", 500000);
    City* c2 = new City("B", 200000);
    Car* car = new Car("Car", 100.0, 8.0);

    Edge* edge = new Edge(c1, c2, 120.0, RoadType::ROAD, RoadCharacteristic::HIGHWAY);

    SUBCASE("Without obstacles") {
        CHECK(edge->calculateTravelTime(*car) == doctest::Approx(1.0));
    }

    SUBCASE("With obstacle") {
        auto* obs = new Obstacle{"Accident", 0.5, ObstacleCause::TRAFFIC_JAM};
        edge->addObstacle(obs);

        CHECK(edge->calculateTravelTime(*car) == doctest::Approx(2.80769));
    }

    SUBCASE("Vehicle cannot use this road") {
        AirVehicle plane("Plane", 900, 3000);
        CHECK(edge->calculateTravelTime(plane) == std::numeric_limits<double>::infinity());
    }

    delete edge;
    delete car;
    delete c1;
    delete c2;
}

TEST_CASE("Route: total distance, time, fuel") {
    City* A = new City("A", 1000000);
    City* B = new City("B", 500000);
    City* C = new City("C", 200000);

    Car* car = new Car("Car", 100.0, 8.0);

    Edge* AB = new Edge(A, B, 100.0, RoadType::ROAD, RoadCharacteristic::STANDARD);
    Edge* BC = new Edge(B, C, 50.0, RoadType::ROAD, RoadCharacteristic::STANDARD);

    Route route(A, C, car);
    route.addEdge(AB);
    route.addEdge(BC);

    CHECK(route.totalDistance() == doctest::Approx(150.0));

    CHECK(route.totalTime(*car) == doctest::Approx(1.5));

    CHECK(route.totalFuel(*car) == doctest::Approx(12.0));

    delete AB;
    delete BC;
    delete A;
    delete B;
    delete C;
    delete car;
}